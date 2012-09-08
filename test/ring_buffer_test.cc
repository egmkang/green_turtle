#include <ring_buffer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


using namespace green_turtle;

int main()
{
  RingBuffer<char> buffer(16);
  const char *message_in = "0123456789";
  char message_out[24];
  int count  = 0;
  while(count++ < 10)
  {
    buffer.Write(message_in,strlen(message_in));
    memset(message_out,0,sizeof(message_out));
    buffer.Read(&message_out[0], sizeof(message_out));
    printf("%s\n",message_out);
  }
  buffer.Write(message_in,strlen(message_in));
  memset(message_out,0,sizeof(message_out));
  buffer.Reset();
  buffer.Read(&message_out[0], sizeof(message_out));
  printf("%s\n",message_out);
  buffer.Write(message_in,strlen(message_in));
  memset(message_out,0,sizeof(message_out));
  buffer.Reset();
  buffer.Read(&message_out[0], sizeof(message_out));
  printf("%s\n",message_out);

  return 0;
}
