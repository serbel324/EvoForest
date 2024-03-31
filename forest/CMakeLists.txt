cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 17)

set(INCPATH
    ${SFML_ROOT}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

project(Forest)

set(SOURCES
    global.cpp
    main.cpp
    time_service.cpp
    world.cpp

    tree/phenotype.cpp
    tree/tree.cpp

    tree/nodes/base.cpp
    tree/nodes/branch.cpp
    tree/nodes/leaf.cpp
    tree/nodes/miner.cpp
    tree/nodes/root.cpp
    tree/nodes/root_sprout.cpp
    tree/nodes/seed.cpp
    tree/nodes/sprout.cpp

    renderer/rengine/tree_render_impl.cpp
    renderer/rengine/world_render_impl.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCPATH}
)

target_link_libraries(${PROJECT_NAME}
    EvoLib
    REngineCore
    REngineDriver
)