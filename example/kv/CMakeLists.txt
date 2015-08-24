INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/green_turtle/deps/sophia/sophia/sophia)

ADD_EXECUTABLE(kv_server
 server.cc
)

ADD_EXECUTABLE(kv_client
 client.cc
)

TARGET_LINK_LIBRARIES(kv_server green_turtle jemalloc)
TARGET_LINK_LIBRARIES(kv_client green_turtle jemalloc)
