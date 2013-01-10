# - CodeCoverage

find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)


set(COVERAGE_DEFINITIONS "-fprofile-arcs -ftest-coverage")
set(COVERAGE_LIBRARIES gcov)


function(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname)
  if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found!")
  endif()

  if(NOT LCOV_PATH)
    message(FATAL_ERROR "lcov not found!")
  endif()
  
  if(NOT GENHTML_PATH)
    message(FATAL_ERROR "genhtml not found!")
  endif()

  # Setup target
  add_custom_target(${_targetname}
    ${LCOV_PATH} --directory . --zerocounters # Cleanup lcov
    COMMAND ${_testrunner} ${ARGV3} # Run tests

    # Capturing lcov counters and generating report
    COMMAND ${LCOV_PATH} --directory . --capture --output-file ${_outputname}.info
    COMMAND ${LCOV_PATH} --remove ${_outputname}.info 'tests/*' '/usr/*' --output-file ${_outputname}.info.cleaned
    COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.info.cleaned
    COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.info ${_outputname}.info.cleaned

    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
    )

  # Show info where to find the report
  add_custom_command(TARGET ${_targetname} POST_BUILD
    COMMAND ;
    COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
    )

endfunction()
