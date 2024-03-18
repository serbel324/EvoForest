cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)

set(INCPATH
    ${PROJECT_SOURCE_DIR}/include)

project(REngineCore)

set(SOURCES
    camera.cpp
    color.cpp
    graphics.cpp
    frame.cpp
    input.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCPATH}
    ${SFML_ROOT}/include
)

target_link_libraries(${PROJECT_NAME}
    GenericLibrary
    sfml-graphics
    sfml-window
    sfml-system)
