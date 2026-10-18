cmake_minimum_required(VERSION 3.16)

project(Rooms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(rooms ${SOURCES})

target_include_directories(rooms PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)