cmake_minimum_required(VERSION 3.31)

project(
        LibraryControl
        DESCRIPTION "Library control coursework app"
        VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(LibraryControl ${SOURCES})
target_include_directories(LibraryControl PRIVATE include lib)
target_link_libraries(LibraryControl PRIVATE sqlite3)
