cmake_minimum_required(VERSION 3.10)

project("rviews" VERSION 0.1.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  add_subdirectory(test)
endif()

if(${PROJECT_NAME}_ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()