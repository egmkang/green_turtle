#include "socket_config.h"

using namespace green_turtle::net;

int32_t SocketConfig::kAcceptBacklogCount  = 128;
int32_t SocketConfig::kInitEventSize = 128;

int32_t SocketConfig::kSocketRecvBufferSize = 128*1024;
int32_t SocketConfig::kSocketSendBufferSize = 128*1024;
int32_t SocketConfig::kClientRecvBufferSize = 16*1024;
int32_t SocketConfig::kClientSendBufferSize = 16*1024;

int32_t SocketConfig::kBufferedSocketBufferSize = 1024;
size_t SocketConfig::kBufferedSocketRetrieveBufferSize = 128;

size_t SocketConfig::kSocketRecvTimeOut = 60*1000;

int32_t SocketConfig::kTimerQueueSlotCount = 2048;
int32_t SocketConfig::kTimerQueueFrameTime = 16;

size_t SocketConfig::kEventLoopFrameTime = 20;
