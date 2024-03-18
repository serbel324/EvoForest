cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)

message(STATUS ${SFML_ROOT})

set(INCPATH
    ${SFML_ROOT}/include
    ${PROJECT_SOURCE_DIR}/include
)

project(EvoLib)

set(SOURCES
    mutator.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCPATH}
)

target_link_libraries(${PROJECT_NAME}
    GenericLibrary
)