ADD_EXECUTABLE(message_queue_test message_queue_test.cc)

ADD_EXECUTABLE(timer_perf_test pq_timer.cpp timer_perf.cpp)

ADD_EXECUTABLE(log_test log_test.cc)

IF(JEMALLOC_LIBRARY)
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle jemalloc)

  TARGET_LINK_LIBRARIES(timer_perf_test green_turtle jemalloc)

  TARGET_LINK_LIBRARIES(log_test green_turtle jemalloc)
ELSE(JEMALLOC_LIBRARY)
  TARGET_LINK_LIBRARIES(message_queue_test green_turtle)

  TARGET_LINK_LIBRARIES(timer_perf_test green_turtle)

  TARGET_LINK_LIBRARIES(log_test green_turtle)
ENDIF(JEMALLOC_LIBRARY)

ADD_EXECUTABLE(timer_test
  timer_test.cc
  )
TARGET_LINK_LIBRARIES(timer_test green_turtle)

IF(HAS_LIBMYSQLCLIENT)
  ADD_EXECUTABLE(mysql_test mysql.cc)
  TARGET_LINK_LIBRARIES(mysql_test green_turtle mysqlclient_r)
ENDIF(HAS_LIBMYSQLCLIENT)

ADD_EXECUTABLE(format_test
  format_test.cc
  )
TARGET_LINK_LIBRARIES(format_test green_turtle)

ADD_EXECUTABLE(xml_doc_test
  xml_doc_test.cc
  )
TARGET_LINK_LIBRARIES(xml_doc_test green_turtle)

ADD_EXECUTABLE(unittest
  test.cc
  unittest_buffer.cc
  unittest_flags.cc
  unittest_unordered_list.cc
  unittest_vector_map.cc
  )
TARGET_LINK_LIBRARIES(unittest green_turtle gtest)


ADD_SUBDIRECTORY(net)

