find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "*.cc")

add_executable(${PROJECT_NAME}-test)
target_sources(${PROJECT_NAME}-test PRIVATE ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME}-test PRIVATE GTest::Main ${PROJECT_NAME})
gtest_discover_tests(${PROJECT_NAME}-test)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}-test> $<TARGET_FILE_DIR:${PROJECT_NAME}-test>
    COMMAND_EXPAND_LISTS
  )
endif()
