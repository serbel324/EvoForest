cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)

set(INCPATH
    ${SFML_ROOT}/include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

project(DSys)

set(SOURCES
    # cpu_test.cpp
    cli.cpp
    cpu.cpp
    cpu_cmd_impl.cpp
    main.cpp
    monitoring.cpp
    os.cpp
    paging.cpp
    process.cpp
    ram.cpp
    stats_tracker.cpp

    messaging/interconnect_proxy.cpp
    messaging/message.cpp
    messaging/message_dump.cpp
    messaging/message_fork.cpp
    messaging/message_block_consumed.cpp
    messaging/message_proxy.cpp
    messaging/message_queue.cpp
)

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${INCPATH}
)

target_link_libraries(${PROJECT_NAME}
    EvoLib
)