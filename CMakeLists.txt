cmake_minimum_required(VERSION 3.20)

project(Typey CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)

add_executable(Typey main.cpp
        Terminal.hpp
        typing.hpp
        colors.hpp
        Time.hpp
        worlds.hpp
        config.hpp)
configure_file(${CMAKE_SOURCE_DIR}/words.txt ${CMAKE_BINARY_DIR}/words.txt COPYONLY)
target_include_directories(Typey PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(Typey PRIVATE ${CURSES_LIBRARIES})

