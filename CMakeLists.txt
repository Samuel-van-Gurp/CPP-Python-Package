cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
project(C_API LANGUAGES C CXX)


# inclucde GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()


# OBJECT library with core implementation
add_library(core_impl OBJECT
    cpp_code/api_impl.cpp
    cpp_code/IO/WriteImage.cpp
    cpp_code/SnakeController.cpp

    cpp_code/DataObjects/Contour.cpp
    cpp_code/Algorithm/ContourResampler.cpp
    cpp_code/Algorithm/ELSnakeEngine.cpp
    cpp_code/Algorithm/GreedySnakeEngine.cpp
    cpp_code/Algorithm/ISnakeEngine.cpp

    cpp_code/ImageProcessing/ImageProcessorFacade.cpp
    cpp_code/ImageProcessing/NaiveConvolve.cpp
    cpp_code/ImageProcessing/IntensityManipulator.cpp
)
target_include_directories(core_impl PRIVATE 
                                        ${CMAKE_SOURCE_DIR}
                                        ${CMAKE_SOURCE_DIR}/cpp_code)

add_library(hello_c_api SHARED
    C_API/api.cpp
)

target_link_libraries(hello_c_api PRIVATE core_impl)
target_include_directories(hello_c_api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(hello_c_api PRIVATE BUILDING_API)


add_executable(runTests
    cpp_code/Tests/DataObjects_Test.cpp
    cpp_code/Tests/Image_Test.cpp
    cpp_code/Tests/ImageHolderTest.cpp
    cpp_code/Tests/SnakeEngine_Test.cpp
    cpp_code/Tests/ELSnakeEngine_Test.cpp
    cpp_code/Tests/ContourResampler_Test.cpp
)

target_include_directories(runTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}          
        ${CMAKE_SOURCE_DIR}/cpp_code 
        ${CMAKE_SOURCE_DIR}/C_API    
)

target_link_libraries(runTests
    PRIVATE
    gtest_main
    core_impl
    hello_c_api 
)