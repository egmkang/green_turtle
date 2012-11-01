#ifndef __BROADCAST_Command__
#define __BROADCAST_Command__
#include <stdint.h>

enum
{
  kCommandMaxLen  = 4*1024,
};

struct Command
{
  explicit Command(uint16_t type) : type(type) {}
  uint16_t  len;
  uint16_t  type;
};

enum CommandType
{
  kCommandType_NULL           = 0,
  kCommandType_Echo           = 1,
  kCommandType_EchoResponse   = 1,
  kCommandType_BroadCast      = 2,
};

template<typename T>
struct PodLength
{
  uint16_t Length() const{ return sizeof(*this); }
};

struct EchoCommand : public Command
{
  EchoCommand() : Command(kCommandType_Echo)
  {
  }
  uint16_t  data_len;
  char      data[0];
  uint16_t  Length() const
  {
    return sizeof(*this) + data_len;
  }
};

struct EchoResponseCommand : public Command, public PodLength<EchoResponseCommand>
{
  EchoResponseCommand() : Command(kCommandType_EchoResponse)
  {
  }
};

struct BroadCastCommand : public Command
{
  BroadCastCommand() : Command(kCommandType_BroadCast)
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
