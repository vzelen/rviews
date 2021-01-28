cmake_minimum_required(VERSION 3.15)

enable_testing()
include(cmake/Utils.cmake)
include(cmake/StandardSettings.cmake)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME})
add_clang_format_target()

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

verbose_message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

#
# Set the sources for the unit tests and add the executable(s)
#

set(test_sources 
  src/test_list_view.cpp
  src/test_vector_view.cpp
  src/test_map_view.cpp
  src/test_unordered_map_view.cpp
)
add_executable(${PROJECT_NAME} ${test_sources})
target_include_directories(${PROJECT_NAME} PUBLIC include/)
add_compile_definitions(VIEW_DEBUG)
  
#
# Set the compiler standard
#

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#
# Setup code coverage if enabled
#

if (${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
  target_compile_options(${CMAKE_PROJECT_NAME} INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${CMAKE_PROJECT_NAME} INTERFACE -fprofile-arcs -ftest-coverage)
  verbose_message("Code coverage is enabled and provided with GCC.")
endif()

#
# Load the desired unit testing framework
#

set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})

find_package(GTest REQUIRED)

if(${CMAKE_PROJECT_NAME}_USE_GOOGLE_MOCK)
  set(GOOGLE_MOCK_LIBRARIES GTest::gmock GTest::gmock_main)
endif()

target_link_libraries(
  ${PROJECT_NAME} 
  PUBLIC
    rviews
    GTest::GTest
    GTest::Main
    ${GOOGLE_MOCK_LIBRARIES}
    ${${CMAKE_PROJECT_NAME}_TEST_LIB}
)

#
# Add the unit tests
#

add_test(
  NAME
    ${PROJECT_NAME}
  COMMAND
    ${PROJECT_NAME}
)

verbose_message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
