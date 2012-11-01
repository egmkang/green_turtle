#include <constructor_in_place.h>
#include "buffered_socket.h"
#include "addr_info.h"
#include "socket_option.h"
#include "event_loop.h"

using namespace green_turtle;
using namespace green_turtle::net;

BufferedSocket::BufferedSocket(int fd,const AddrInfo& addr, int recv_buff, int send_buff)
  : EventHandler(fd)
    , addr_(addr)
    , rcv_buffer_(nullptr)
    , write_lock_()
{
  if(recv_buff)
  {
    SocketOption::SetRecvBuffer(fd, recv_buff);
  }
  rcv_buffer_ = new CacheLine(SocketOption::GetRecvBuffer(fd));
  if(send_buff)
  {
    SocketOption::SetSendBuffer(fd, send_buff);
  }
}

BufferedSocket::~BufferedSocket()
{
  this->snd_queue_.clear();
  this->snd_raw_data_queue.clear();
  delete rcv_buffer_;
}
const AddrInfo& BufferedSocket::addr() const
{
  return this->addr_;
}

int BufferedSocket::OnRead()
{
  int nread = SocketOption::Read(fd(), rcv_buffer_->BeginWrite(), rcv_buffer_->WritableLength());
  if(nread < 0)
    return kErr;
  if(nread)
  {
    rcv_buffer_->HasWritten(nread);
    Decoding(*rcv_buffer_);
    rcv_buffer_->Retrieve();
  }
  return kOK;
}

int BufferedSocket::OnWrite()
{
  if(this->snd_raw_data_queue.empty() && this->snd_queue_.empty())
    return kOK;
  std::deque<RawData> temp_send_queue;
  {
    std::lock_guard<std::mutex> lock(this->write_lock_);
    this->snd_raw_data_queue.swap(temp_send_queue);
  }

  for(auto& message: temp_send_queue)
  {
    this->snd_queue_.push_back(message);
  }

  //send raw data using writev
  std::vector<iovec> send_datas_;
  for(const auto& message : snd_queue_)
  {
    iovec value = {message->data(), message->length()};
    send_datas_.push_back(value);
    if(send_datas_.size() >= IOV_MAX) break;
  }
  if(this->current_send_)
  {
    char    *data = (char*)send_datas_[0].iov_base;
    size_t  len = send_datas_[0].iov_len;
    assert(this->current_send_ < len);
    data += this->current_send_;
    len -= this->current_send_;
    send_datas_[0].iov_base = data;
    send_datas_[0].iov_len = len;
  }

  int send_size = SocketOption::Writev(fd(), &send_datas_[0], send_datas_.size());
  if(send_size < 0)   return kErr;
  else if(!send_size) return kOK;

  for(const auto& io : send_datas_)
  {
    if(send_size >= (int)io.iov_len)
    {
      this->snd_queue_.pop_front();
      send_size -= io.iov_len;
    }
    else
    {
      break;
    }
  }
  this->current_send_ = send_size;
  return kOK;
}

int BufferedSocket::OnError()
{
  this->event_loop()->RemoveEventHandler(this);
  this->DeleteSelf();
  return -1;
}

void BufferedSocket::SendMessage(std::shared_ptr<Message> &message)
{
  std::lock_guard<std::mutex> lock(this->write_lock_);
  this->snd_raw_data_queue.push_back(message);
}

