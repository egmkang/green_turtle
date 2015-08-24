CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(green_turtle)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/green_turtle)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/gtest/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/lz4)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps/pugixml/src)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckClang.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckLibCXX.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckCXXCompilerFlag.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/CheckLibMySQLClient.cmake)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()


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
    -DPUGIXML_NO_EXCEPTIONS
    -pthread
    )

IF(HAS_CLANG)
  SET(CXX_FLAGS ${CXX_FLAGS}
    -Wno-unused-const-variable
    -fdiagnostics-format=vi
    )
ELSE(HAS_CLANG)
  SET(CXX_FLAGS ${CXX_FLAGS}
    -Wl,--no-as-needed
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

SET(CMAKE_CXX_FLAGS_DEBUG   "-DDEBUG -D_DEBUG -D_LIBCPP_DEBUG2=1 -O0")
SET(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

FIND_LIBRARY(JEMALLOC_LIBRARY NAMES jemalloc)

ADD_SUBDIRECTORY(deps)
ADD_SUBDIRECTORY(green_turtle)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(example)
