
IF(JEMALLOC_LIBRARY)
  ADD_EXECUTABLE(message_queue_test
    message_queue_test.cc
    )
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle jemalloc)
ELSE(JEMALLOC_LIBRARY)
  ADD_EXECUTABLE(message_queue_test
    message_queue_test.cc
    )
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle)
ENDIF(JEMALLOC_LIBRARY)


ADD_EXECUTABLE(timer_test
  timer_test.cc
  )
TARGET_LINK_LIBRARIES(timer_test green_turtle)

ADD_EXECUTABLE(timer_perf_test
    pq_timer.cpp
    timer_perf.cpp
    )
TARGET_LINK_LIBRARIES(timer_perf_test green_turtle jemalloc)

ADD_EXECUTABLE(log_test
    log_test.cc
    )
TARGET_LINK_LIBRARIES(log_test green_turtle jemalloc)

ADD_EXECUTABLE(mysql_test
  mysql.cc
    )
TARGET_LINK_LIBRARIES(mysql_test green_turtle mysqlclient)


ADD_SUBDIRECTORY(net)
