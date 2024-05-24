find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "*.cc")

add_executable(${PROJECT_NAME}-test)
set_target_properties(${PROJECT_NAME}-test PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_sources(${PROJECT_NAME}-test PRIVATE ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}-test PRIVATE GTest::Main ${PROJECT_NAME})

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}-test> $<TARGET_FILE_DIR:${PROJECT_NAME}-test>
    COMMAND_EXPAND_LISTS
  )

  # copy lib${PROJECT_NAME}.dll folder with ${PROJECT_NAME}-test.exe
  message(STATUS "copying ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll to ${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll")
  add_custom_command(TARGET ${PROJECT_NAME}-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.dll" "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dll"
    COMMAND_EXPAND_LISTS
  )
endif()

gtest_discover_tests(${PROJECT_NAME}-test)