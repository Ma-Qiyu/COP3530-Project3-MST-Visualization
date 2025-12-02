cmake_minimum_required(VERSION 4.0)
project(COP3530_Final_Project_MST)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SFML_DIR "/opt/homebrew/opt/sfml@2/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

include_directories(.)

include_directories(${SFML_INCLUDE_DIRS})
include_directories(lib)

add_executable(COP3530_Final_Project_MST
        Graph.cpp
        Graph.h
        main.cpp
        MSTBuilder.cpp
        MSTBuilder.h
        UIController.cpp
        UIController.h
        TextInputBox.cpp
        TextInputBox.h)

target_link_libraries(COP3530_Final_Project_MST sfml-system sfml-window sfml-graphics)

add_custom_command(TARGET COP3530_Final_Project_MST POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/lib/Roboto-Regular.ttf"
        "$<TARGET_FILE_DIR:COP3530_Final_Project_MST>/font.ttf"
        COMMENT "Copying font file")
