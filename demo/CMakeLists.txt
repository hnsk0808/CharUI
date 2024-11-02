cmake_minimum_required(VERSION 3.12)

project(demo
    LANGUAGES CXX
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)

add_executable(demo
    demo.cpp
)

target_include_directories(demo PUBLIC
    ../include/
)

target_link_libraries(demo PUBLIC
    CharUI
)