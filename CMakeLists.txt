cmake_minimum_required(VERSION 3.31)

project(Examples
        VERSION 0.1
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(vendor)
add_subdirectory(example-01)
add_subdirectory(example-02)
add_subdirectory(example-03)