ADD_EXECUTABLE(echo_server
    echo_server.cc
    )
TARGET_LINK_LIBRARIES(echo_server green_turtle tcmalloc)

ADD_EXECUTABLE(echo_client
    echo_client.cc
    )
TARGET_LINK_LIBRARIES(echo_client green_turtle tcmalloc)
