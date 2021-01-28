cmake_minimum_required(VERSION 3.15)

add_executable(rviewsExample views_example.cpp)

target_compile_features(rviewsExample PUBLIC cxx_std_17)

target_link_libraries(rviewsExample PUBLIC rviews)