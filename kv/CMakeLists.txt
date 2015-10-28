INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/sophia/sophia/sophia)
MESSAGE(${CMAKE_SOURCE_DIR}/deps/sophia/sophia/sophia)

ADD_EXECUTABLE(kv_server
 server.cc
 message_loop.cc
 bench.cc
)

ADD_EXECUTABLE(kv_client
 client.cc
)

SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH ".")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

TARGET_LINK_LIBRARIES(kv_server green_turtle sophia jemalloc)
TARGET_LINK_LIBRARIES(kv_client green_turtle jemalloc)
