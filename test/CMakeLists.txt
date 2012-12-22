
IF(TCMALLOC_LIBRARY OR JEMALLOC_LIBRARY)
    IF(TCMALLOC_LIBRARY)
        ADD_EXECUTABLE(message_queue_test_tc
            message_queue_test.cc
            )
        TARGET_LINK_LIBRARIES(message_queue_test_tc green_turtle tcmalloc)
    ENDIF(TCMALLOC_LIBRARY)
    IF(JEMALLOC_LIBRARY)
        ADD_EXECUTABLE(message_queue_test_je
            message_queue_test.cc
            )
        TARGET_LINK_LIBRARIES(message_queue_test_je green_turtle jemalloc)
    ENDIF(JEMALLOC_LIBRARY)
ELSE(TCMALLOC_LIBRARY OR JEMALLOC_LIBRARY)
    ADD_EXECUTABLE(message_queue_test
        message_queue_test.cc
        )
    TARGET_LINK_LIBRARIES(message_queue_test green_turtle)
ENDIF(TCMALLOC_LIBRARY OR JEMALLOC_LIBRARY)


ADD_EXECUTABLE(timer_test
    timer_test.cc
    )
TARGET_LINK_LIBRARIES(timer_test green_turtle)

ADD_EXECUTABLE(unordered_list_test
    unordered_list_test.cc
    )
TARGET_LINK_LIBRARIES(unordered_list_test green_turtle)

ADD_EXECUTABLE(hash_test
    hash_test.cc
    )
TARGET_LINK_LIBRARIES(hash_test green_turtle jemalloc)

ADD_SUBDIRECTORY(net)
