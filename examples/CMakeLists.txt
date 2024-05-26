set(EXAMPLE_NAME_1 "${PROJECT_NAME}-example-read-process-memory")
add_executable(${EXAMPLE_NAME_1} ${CMAKE_CURRENT_SOURCE_DIR}/read_process_memory.cc)

set(EXAMPLE_NAME_2 "${PROJECT_NAME}-example-call-external-function")
add_executable(${EXAMPLE_NAME_2} ${CMAKE_CURRENT_SOURCE_DIR}/call_external_function.cc)

set_target_properties(${EXAMPLE_NAME_1} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

set_target_properties(${EXAMPLE_NAME_2} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_link_libraries(${EXAMPLE_NAME_1} PRIVATE ${PROJECT_NAME})
target_link_libraries(${EXAMPLE_NAME_2} PRIVATE ${PROJECT_NAME})

if(WIN32)
  add_custom_command(TARGET ${EXAMPLE_NAME_1}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${EXAMPLE_NAME_1}> $<TARGET_FILE_DIR:${EXAMPLE_NAME_1}>
    COMMAND_EXPAND_LISTS
  )

  # copy lib${PROJECT_NAME}.dll folder with ${PROJECT_NAME}-test.exe
  message(STATUS "copying ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll to ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll")
  add_custom_command(TARGET ${EXAMPLE_NAME_1}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll" "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll"
    COMMAND_EXPAND_LISTS
  )
endif()