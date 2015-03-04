ADD_EXECUTABLE(echo_server
    echo_server.cc
    )

ADD_EXECUTABLE(echo_client
    echo_client.cc
    )

ADD_EXECUTABLE(broadcast_server
    broadcast_server.cc
    )

ADD_EXECUTABLE(broadcast_client
    broadcast_client.cc
    )

TARGET_LINK_LIBRARIES(echo_server green_turtle jemalloc)
TARGET_LINK_LIBRARIES(echo_client green_turtle jemalloc)
TARGET_LINK_LIBRARIES(broadcast_server green_turtle jemalloc)
TARGET_LINK_LIBRARIES(broadcast_client green_turtle jemalloc)

