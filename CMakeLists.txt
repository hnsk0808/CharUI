cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

project(CharUI
    LANGUAGES CXX C
    VERSION 0.0.3
)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS 
    include/CharUI/*.h
    src/*.cpp
)

file(GLOB_RECURSE thirdparty CONFIGURE_DEPENDS
    thirdparty/*.h
    thirdparty/*.hpp
    thirdparty/*.c
    thirdparty/*.cpp
)

add_library(ThirdParty
    ${thirdparty}
)

add_library(CharUI
    ${sources}
)

target_include_directories(CharUI PUBLIC
    include/
    thirdparty/
)

target_link_libraries(CharUI PUBLIC 
    ThirdParty
)

add_subdirectory(demo)