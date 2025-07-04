cmake_minimum_required(VERSION 3.20)
project(oct LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(
    include
    third_party/json/single_include
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    include/**/*.hpp
)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(oct ${SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCES})
