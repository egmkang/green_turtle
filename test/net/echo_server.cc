#include <net/tcp_acceptor.h>
#include <net/tcp_server.h>
#include <net/buffered_socket.h>
#include <net/event_handler_factory.h>
#include <stdlib.h>
#include <stdio.h>

using namespace green_turtle;
using namespace green_turtle::net;

class EchoTask : public BufferedSocket
{
 public:
  EchoTask(int fd,const AddrInfo& addr):BufferedSocket(fd, addr){}
 protected:
  virtual void ProcessInputData(CacheLine& data)
  {
    size_t size = data.GetSize();
    if(size)
    {
      void *data_copy = malloc(size);
      data.Read((unsigned char*)data_copy, size);
      this->SendMessage(data_copy, size);
    }
  }
  virtual void ProcessOutputMessage(const void *data, unsigned int len)
  {
    size_t sent = 0;
    while(len - sent)
    {
      CacheLine *cache = GetNewCacheLine();
      sent += cache->Write(reinterpret_cast<const unsigned char*>(data) +
                           sent, len - sent);
    }
    free((void*)data);
  }
  virtual void ProcessDeleteSelf()
  {
    printf("EchoTask will be disposed, %p\n", this);
    delete this;
  }
};

EventHandler* NewEventHanlder(int fd, const AddrInfo& addr)
{
  return new EchoTask(fd, addr);
}

int main()
{
  TcpAcceptor acceptor("127.0.0.1", 10001, 16*1024, 16*1024);
  bool result = acceptor.Listen();
  assert(result);

  EventHandlerFactory::Instance().RegisterDefault(&NewEventHanlder);
  TcpServer server(16);
  server.AddAcceptor(&acceptor);
  //server.SetThreadCount(2);
  server.Run();
  return 0;
}

