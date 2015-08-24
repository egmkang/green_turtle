#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include <stdint.h>
#include <string>

//magic code
enum {
  PROTOCOL_REQUEST  = 0x80,
  PROTOCOL_RESPONSE = 0x81,
};

//opcode
enum {
 PROTOCOL_CMD_VERSION = 0x01, //REQUEST: no body; RESPONSE: one string
 PROTOCOL_CMD_NOOP    = 0x02, //REQUEST: no body; RESPONSE: no body
 PROTOCOL_CMD_ERROR   = 0x03, //RESPONSE: one byte
 PROTOCOL_CMD_GET     = 0x04, //REQUEST: one string; RESPONSE: one string
 PROTOCOL_CMD_SET     = 0x05, //REQUEST: one string; RESPONSE: no body
 PROTOCOL_CMD_DEL     = 0x06, //REQUEST: one string; RESPONSE: no body
 PROTOCOL_CMD_APPEND  = 0x07, //REQUEST: one string; RESPONSE: no body
 PROTOCOL_CMD_INC     = 0x08, //REQUEST: one string; RESPONSE: one string(int64_t default 1)
 PROTOCOL_CMD_DEC     = 0x09, //REQUEST: one string; RESPONSE: one string(int64_t default 1)
 PROTOCOL_CMD_FLUSH   = 0x0A, //REQUEST: no body; RESPONSE: no body
};

enum {
 PROTOCOL_ERR_OK        = 0,
 PROTOCOL_ERR_KEY_VALID = 1, //key not exist
};

#pragma pack (1)

//little endian
struct MessageHead {
 uint32_t length;
 uint8_t magic;
 uint8_t opcode;
 uint16_t affinity;
 uint64_t cas;
 uint32_t seq;
};

#pragma pack ()

//little endian
//4byte len; raw data
std::string DecodeString(void **ptr);
void EncodeString(void **ptr, const std::string& str);

#endif
