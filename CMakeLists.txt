cmake_minimum_required(VERSION 3.15)
project(C_API LANGUAGES C CXX)

# add_library(hello_c_api SHARED C_API/api.c cpp)
add_library(hello_c_api SHARED C_API/api.c 
                               cpp_code/halloWorld.cpp)
target_include_directories(hello_c_api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(hello_c_api PRIVATE BUILDING_API)