CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(green_turtle)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/green_turtle)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/gtest-1.7.0)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/gtest-1.7.0/include)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckClang.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckLibCXX.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckCXXCompilerFlag.cmake)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

SET(CMAKE_CXX_COMPILER "clang++")

SET(CXX_FLAGS
    -g
    -Wall
    -Wextra
    -Werror
    -std=c++11
    -msse4.2
    -fomit-frame-pointer
    -fstack-protector-all
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -fno-rtti
    -fno-exceptions
    -D__STDC_FORMAT_MACROS
    -DGTEST_HAS_RTTI=0
    -DGTEST_HAS_EXCEPTIONS=0
    )

IF(HAS_CLANG)
  SET(CXX_FLAGS ${CXX_FLAGS}
    -Wno-unused-const-variable
    -fdiagnostics-format=vi
    )
ENDIF(HAS_CLANG)

IF(HAS_CLANG AND HAS_LIBCXX)
  MESSAGE(STATUS "using libc++")
  SET(CXX_FLAGS ${CXX_FLAGS}
    -stdlib=libc++
    )
ENDIF(HAS_CLANG AND HAS_LIBCXX)

STRING(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
MESSAGE(STATUS "CXX flags : " "${CMAKE_CXX_FLAGS}")

SET(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -D_DEBUG -D_LIBCPP_DEBUG2=1 -O0 -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3 -pthread")

FIND_LIBRARY(JEMALLOC_LIBRARY NAMES jemalloc)

ADD_SUBDIRECTORY(deps/gtest-1.7.0)
ADD_SUBDIRECTORY(green_turtle)
ADD_SUBDIRECTORY(test)
