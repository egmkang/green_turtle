#gtest
INCLUDE_DIRECTORIES(gtest)
INCLUDE_DIRECTORIES(gtest/include)

FILE(GLOB GTEST gtest/src/gtest-all.cc gtest/src/*.h gtest/include/gtest/*.h gtest/include/gtest/internal/*.h)

SOURCE_GROUP("gtest"      FILES ${GTEST})

ADD_LIBRARY(gtest STATIC
    ${GTEST}
    )
TARGET_LINK_LIBRARIES(gtest pthread)


#pugixml
INCLUDE_DIRECTORIES(./pugixml/src)

SET(PUGI_HEADERS ./pugixml/src/pugixml.hpp ./pugixml/src/pugiconfig.hpp)
SET(PUGI_SOURCES ${PUGI_HEADERS} ./pugixml/src/pugixml.cpp)

ADD_LIBRARY(pugixml STATIC ${PUGI_SOURCES})
SET_TARGET_PROPERTIES(pugixml PROPERTIES
  VERSION 1.4 SOVERSION 1
  COMPILE_FLAGS "-O3"
  )

SET(LZ4VERSION 123)

#lz4
INCLUDE_DIRECTORIES(./lz4)
FILE(GLOB LZ4_SOURCES lz4/*.h lz4/*.c)
SOURCE_GROUP("lz4_sources"         FILES ${LZ4_SOURCES})
ADD_LIBRARY(lz4 STATIC ${LZ4_SOURCES})
SET_TARGET_PROPERTIES(lz4 PROPERTIES
  VERSION ${LZ4VERSION}
  COMPILE_FLAGS "-O3 -g -std=c99 -Wall -Wextra -Wundef -Wshadow -Wstrict-prototypes -DLZ4_VERSION=\"${LZ4VERSION}\""
  )

ADD_SUBDIRECTORY(sophia)
