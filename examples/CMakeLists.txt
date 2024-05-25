set(EXAMPLE_NAME_1 "${PROJECT_NAME}-example-read-process-memory")
add_executable(${EXAMPLE_NAME_1})

set_target_properties(${EXAMPLE_NAME_1} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_sources(${EXAMPLE_NAME_1} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/read_process_memory.cc
)

target_link_libraries(${EXAMPLE_NAME_1} PRIVATE ${PROJECT_NAME})