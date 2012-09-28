#include <net/tcp_acceptor.h>
#include <net/tcp_server.h>
#include <net/buffered_socket.h>
#include <net/event_handler_factory.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <atomic>
#include <system.h>
#include <iostream>

using namespace green_turtle;
using namespace green_turtle::net;

size_t              last_update_time_ = 0;
std::atomic<size_t> message_count(0);
size_t              last_update_message_count = 0;
std::atomic<size_t> message_size(0);
size_t              last_update_message_size = 0;
