cmake_minimum_required(VERSION 3.12)

project(CharUI
    LANGUAGES CXX C
    VERSION 1.0.0
)

set(CMAKE_CXX_STANDARD 20)

add_library(CharUI
    include/CharUI/Component/Canvas.h
    include/CharUI/Component/Component.h
    include/CharUI/Component/Components.h
    include/CharUI/Component/Grid.h
    include/CharUI/Component/HBox.h
    include/CharUI/Component/Image.h
    include/CharUI/Component/ProgressBar.h
    include/CharUI/Component/Text.h
    include/CharUI/Component/VBox.h
    include/CharUI/Utils/Chars.h
    include/CharUI/Utils/Pos.h
    include/CharUI/Utils/Signal.h
    include/CharUI/Utils/String.h
    include/CharUI/Utils/Terminal.h
    include/CharUI/Config.h
    include/CharUI/Page.h
    include/CharUI/CharUI.h
    thirdparty/wtswidth/wtswidth.c
    src/Canvas.cpp
    src/Chars.cpp
    src/Config.cpp
    src/Grid.cpp
    src/HBox.cpp
    src/Image.cpp
    src/Page.cpp
    src/ProgressBar.cpp
    src/String.cpp
    src/Terminal.cpp
    src/Text.cpp
    src/VBox.cpp
)

target_include_directories(CharUI PUBLIC
    include/
    thirdparty/
)

add_subdirectory(demo)