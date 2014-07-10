INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

FILE(GLOB UNIT_TEST          *.cc)

SOURCE_GROUP("unittest"      FILES ${UNIT_TEST})


ADD_EXECUTABLE(unittest
    ${UNIT_TEST}
    )
TARGET_LINK_LIBRARIES(unittest green_turtle gtest)

