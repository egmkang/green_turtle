#ifndef __simple_message__
#define __simple_message__
#include <net/message.h>
#include <string>

class SimpleMessage : public green_turtle::net::Message
{
 public:
  SimpleMessage(const char *data)
      :data_(data)
  {
  }
  SimpleMessage(const char *data, size_t len)
      :data_(data, data+len)
  {
  }
  void* data() const { return (void*)&data_[0]; }
  size_t length() const { return data_.length(); }
 private:
  std::string data_;
};

#endif
