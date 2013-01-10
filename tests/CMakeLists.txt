function(rock_test NAME)
  add_executable(test_${NAME} ${PROJECT_SOURCE_DIR}/tests/${NAME}.cpp)
  target_link_libraries(test_${NAME} ${GTEST_LIBRARIES} ${COVERAGE_LIBRARIES} pthread)
  add_test(test_${NAME} test_${NAME})
endfunction()

rock_test(dmp)
rock_test(chain)
rock_test(list)
