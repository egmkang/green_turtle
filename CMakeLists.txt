CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(green_turtle)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/green_turtle)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

IF(NOT CMAKE_BUILD_TYPE)
        SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

SET(CXX_FLAGS
    -g
    -Wall
    -Wextra
    -Werror
    -std=c++11
    -stdlib=libc++
    -fomit-frame-pointer
    -fstack-protector-all
    -fno-rtti
    -fno-exceptions
    )
STRING(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
MESSAGE(STATUS "CXX flags : " "${CMAKE_CXX_FLAGS}")

SET(CMAKE_CXX_COMPILER "clang++")
SET(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -D_DEBUG -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O2")

ADD_SUBDIRECTORY(green_turtle)
ADD_SUBDIRECTORY(test)
