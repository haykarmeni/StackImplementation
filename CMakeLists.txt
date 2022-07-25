cmake_minimum_required(VERSION 3.22)
project(StackImplementation)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-main)

include_directories(lib/googletest-main/googletest/include)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(StackImplementation main.cpp stack.h)
target_link_libraries(StackImplementation gtest gtest_main)