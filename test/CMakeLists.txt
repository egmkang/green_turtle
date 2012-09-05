ADD_EXECUTABLE(message_queue_test
    message_queue_test.cc
    )
TARGET_LINK_LIBRARIES(message_queue_test green_turtle pthread tcmalloc)
