CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(green_turtle)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/green_turtle)

#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall -Werror -std=c++11 -stdlib=libc++ -DDEBUG -D_DEBUG -O0 -fomit-frame-pointer -fstack-protector-all")
set(CMAKE_CXX_FLAGS_RELEASE "-g -Wall -Werror -std=c++11 -stdlib=libc++ -DNDEBUG -O2 -fomit-frame-pointer -fstack-protector-all")

ADD_SUBDIRECTORY(green_turtle)
ADD_SUBDIRECTORY(test)
