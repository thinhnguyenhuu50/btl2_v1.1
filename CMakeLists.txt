cmake_minimum_required(VERSION 3.15)
project(btl2 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB TEST_SOURCES 
    "src/test/tc_heap.cpp"
    # "src/test/tc_compressor.cpp"
    # "src/test/tc_xmap.cpp"
)
add_executable(btl2 src/main.cpp ${TEST_SOURCES})

target_include_directories(btl2 PRIVATE include)