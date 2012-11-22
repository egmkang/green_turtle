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

IF(TCMALLOC_LIBRARY)
    TARGET_LINK_LIBRARIES(echo_server green_turtle tcmalloc)
    TARGET_LINK_LIBRARIES(echo_client green_turtle tcmalloc)
    TARGET_LINK_LIBRARIES(broadcast_server green_turtle tcmalloc)
    TARGET_LINK_LIBRARIES(broadcast_client green_turtle tcmalloc)
ELSEIF(JEMALLOC_LIBRAYR)
    TARGET_LINK_LIBRARIES(echo_server green_turtle jemalloc)
    TARGET_LINK_LIBRARIES(echo_client green_turtle jemalloc)
    TARGET_LINK_LIBRARIES(broadcast_server green_turtle jemalloc)
    TARGET_LINK_LIBRARIES(broadcast_client green_turtle jemalloc)
ELSE()
    TARGET_LINK_LIBRARIES(echo_server green_turtle)
    TARGET_LINK_LIBRARIES(echo_client green_turtle)
    TARGET_LINK_LIBRARIES(broadcast_server green_turtle)
    TARGET_LINK_LIBRARIES(broadcast_client green_turtle)
ENDIF(TCMALLOC_LIBRARY)

