ADD_EXECUTABLE(message_queue_test_tc
    message_queue_test.cc
    )
TARGET_LINK_LIBRARIES(message_queue_test_tc green_turtle tcmalloc)

ADD_EXECUTABLE(message_queue_test_je
    message_queue_test.cc
    )
TARGET_LINK_LIBRARIES(message_queue_test_je green_turtle jemalloc)

ADD_EXECUTABLE(ref_obj_test
    ref_obj_test.cc
    )
TARGET_LINK_LIBRARIES(ref_obj_test green_turtle)

ADD_EXECUTABLE(buffer_test
    buffer_test.cc
    )

ADD_EXECUTABLE(time_test
    time.cc
    )
TARGET_LINK_LIBRARIES(time_test green_turtle)

ADD_EXECUTABLE(timer_test
    timer_test.cc
    )
TARGET_LINK_LIBRARIES(timer_test green_turtle)

ADD_EXECUTABLE(unordered_list_test
    unordered_list_test.cc
    )
TARGET_LINK_LIBRARIES(unordered_list_test green_turtle)

ADD_SUBDIRECTORY(net)
