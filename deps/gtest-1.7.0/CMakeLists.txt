INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(include)

FILE(GLOB GTEST src/gtest-all.cc src/*.h include/gtest/*.h include/gtest/internal/*.h)

SOURCE_GROUP("gtest"      FILES ${GTEST})

ADD_LIBRARY(gtest STATIC
    ${GTEST}
    )
TARGET_LINK_LIBRARIES(gtest pthread)
