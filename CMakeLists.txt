cmake_minimum_required(VERSION 3.31)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.cpp")

project(
    LibraryControl
    DESCRIPTION "Library control coursework app"
    VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 26)

add_executable(LibraryControl ${SOURCES} ${HEADERS})
target_include_directories(LibraryControl PRIVATE include lib)
target_link_libraries(LibraryControl PRIVATE sqlite3)
