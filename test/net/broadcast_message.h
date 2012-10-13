#ifndef __BROADCAST_MESSAGE__
#define __BROADCAST_MESSAGE__
#include <stdint.h>

struct Message
{
  explicit Message(uint16_t type) : type(type) {}
  uint16_t  len;
  uint16_t  type;
};

enum MessageType
{
  kMessageType_NULL           = 0,
  kMessageType_Echo           = 1,
  kMessageType_EchoResponse   = 1,
  kMessageType_BroadCast      = 2,
};

template<typename T>
struct PodLength
{
  uint16_t Length() const{ return sizeof(*this); }
};

struct EchoMessage : public Message
{
  EchoMessage() : Message(kMessageType_Echo)
  {
  }
  uint16_t  data_len;
  char      data[0];
  uint16_t  Length() const
  {
    return sizeof(*this) + data_len;
  }
};

struct EchoResponseMessage : public Message, public PodLength<EchoResponseMessage>
{
  EchoResponseMessage() : Message(kMessageType_EchoResponse)
  {
  }
};

struct BroadCastMessage : public Message
{
  BroadCastMessage() : Message(kMessageType_BroadCast)
  {
  }
  uint16_t  data_len;
  char      data[0];
  uint16_t  Length() const
  {
    return sizeof(*this) + data_len;
  }
};

#endif
