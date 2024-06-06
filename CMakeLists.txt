cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(winapi20
  VERSION 0.0.2
  DESCRIPTION "Windows API wrapper in modern C++20"
  HOMEPAGE_URL "https://github.com/whs31/winapi20"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE winapi::)

option(TESTS "Enable tests" OFF)
option(EXAMPLES "Enable examples" OFF)
option(BUILD_SHARED_LIBS "Build shared library" ON)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CMakePrintHelpers)
find_package(fmt)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAMESPACE}${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

cmake_print_properties(
  TARGETS ${PROJECT_NAME}
  PROPERTIES
    CXX_STANDARD
    CXX_STANDARD_REQUIRED
    CXX_EXTENSIONS
    POSITION_INDEPENDENT_CODE
)

target_sources(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/winapi.h> $<INSTALL_INTERFACE:include/winapi20/winapi.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/export.h> $<INSTALL_INTERFACE:include/winapi20/detail/export.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/definitions.h> $<INSTALL_INTERFACE:include/winapi20/detail/definitions.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/cvt.h> $<INSTALL_INTERFACE:include/winapi20/detail/cvt.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/exception.h> $<INSTALL_INTERFACE:include/winapi20/detail/exception.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/windows_headers.h> $<INSTALL_INTERFACE:include/winapi20/detail/windows_headers.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/template_util.h> $<INSTALL_INTERFACE:include/winapi20/detail/template_util.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/detail/formatters.h> $<INSTALL_INTERFACE:include/winapi20/detail/formatters.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/winbase_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/winbase_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/errhandlingapi_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/errhandlingapi_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/shlobj_core_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/shlobj_core_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/consoleapi_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/consoleapi_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/tlhelp32_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/tlhelp32_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/processthreadsapi_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/processthreadsapi_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/impl/memoryapi_impl.h> $<INSTALL_INTERFACE:include/winapi20/impl/memoryapi_impl.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/wrappers/detail/pointerlike.h> $<INSTALL_INTERFACE:include/winapi20/wrappers/detail/pointerlike.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/wrappers/handle.h> $<INSTALL_INTERFACE:include/winapi20/wrappers/handle.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/wrappers/memaddr.h> $<INSTALL_INTERFACE:include/winapi20/wrappers/memaddr.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/wrappers/function.h> $<INSTALL_INTERFACE:include/winapi20/wrappers/function.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20/wrappers/pid.h> $<INSTALL_INTERFACE:include/winapi20/wrappers/pid.h>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/winbase_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/errhandlingapi_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/shlobj_core_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/consoleapi_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/tlhelp32_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/processthreadsapi_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/memoryapi_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/libloaderapi_impl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/wrappers.cc
)

target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)

message(STATUS "[${PROJECT_NAME}] compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message(STATUS "[${PROJECT_NAME}] linking with additional libraries for msvc")
  find_library(PSAPI Psapi)
  # shell32
  find_library(SHELL32 shell32)
  target_link_libraries(${PROJECT_NAME} PUBLIC psapi shell32)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<BOOL:${BUILD_SHARED_LIBS}>:-DWINAPI20_LIBRARY=1>
  $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:-DWINAPI20_STATIC_LIBRARY=1>
)
cmake_print_variables(BUILD_SHARED_LIBS)

# https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(TESTS)
  if(WIN32)
    enable_testing()
    add_subdirectory(tests)
  else()
    message(STATUS "[${PROJECT_NAME}] tests are not supported on non-windows platform")
  endif()
endif()

if(EXAMPLES)
  if(WIN32)
    add_subdirectory(examples)
  else()
    message(STATUS "[${PROJECT_NAME}] examples are not supported on non-windows platform")
  endif()
endif()

message(STATUS "[${PROJECT_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
message(STATUS "[${PROJECT_NAME}] installing in prefix: ${CMAKE_INSTALL_PREFIX}")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE} FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-targets.cmake")
install(EXPORT ${PROJECT_NAME}
  FILE "${PROJECT_NAME}-config-targets.cmake"
  NAMESPACE ${PROJECT_NAMESPACE}
  DESTINATION lib/cmake/${PROJECT_NAME}
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  @ONLY
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/winapi20> $<INSTALL_INTERFACE:include/winapi20> ;'
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)

message(STATUS "[${PROJECT_NAME}] configuring ${PROJECT_NAME} done!")

unset(TESTS CACHE)
unset(EXAMPLES CACHE)
unset(BUILD_SHARED_LIBS CACHE)