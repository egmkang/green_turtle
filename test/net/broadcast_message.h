#ifndef __BROADCAST_MESSAGE__
#define __BROADCAST_MESSAGE__
#include <stdint.h>

struct Command
{
  Command(uint16_t type, uint16_t len):
      len(len), type(type)
  {
  }
  uint16_t  len;
  uint16_t  type;
};

struct BroadCastCommand : public Command
{
  
};

#endif
