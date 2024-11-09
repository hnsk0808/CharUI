cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)

project(demo
    LANGUAGES CXX
    VERSION 1.0.0
)

add_executable(demo
    demo.cpp
)

target_include_directories(demo PUBLIC
    ../include/
)

target_link_libraries(demo PUBLIC
    CharUI
)