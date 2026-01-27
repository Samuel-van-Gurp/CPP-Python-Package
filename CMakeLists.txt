cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
project(C_API LANGUAGES C CXX)


find_package(Python3 COMPONENTS Interpreter Development REQUIRED)


include(FetchContent)
#  include pybind11
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v3.0.1 
    )
FetchContent_MakeAvailable(pybind11)

# include GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

enable_testing()

# STATIC library with core implementation
add_library(core_impl STATIC
    cpp_code/MagicNumbers.hpp
    cpp_code/IO/WriteImage.cpp
    cpp_code/SnakeController.cpp

    cpp_code/DataObjects/Contour.cpp
    cpp_code/DataObjects/SnakeParams.hpp
    cpp_code/Algorithm/ContourResampler.cpp
    cpp_code/Algorithm/ELSnakeEngine.cpp
    cpp_code/Algorithm/GreedySnakeEngine.cpp
    cpp_code/Algorithm/ISnakeEngine.cpp

    cpp_code/ImageProcessing/ImageProcessorFacade.cpp
    cpp_code/ImageProcessing/NaiveConvolve.cpp
    cpp_code/ImageProcessing/IntensityManipulator.cpp
)

pybind11_add_module(pybindings cpp_code/bindings.cpp)  # create pybindings module

# Include directories from cpp_code for pybindings target
# target_include_directories(pybindings PRIVATE
#     ${PROJECT_SOURCE_DIR}/cpp_code
# )

# Link core_impl to pybindings target
target_link_libraries(pybindings PRIVATE core_impl)

#  Link core_impl and pybind11::module to pybindings target
target_link_libraries(pybindings PRIVATE core_impl pybind11::module)

# Include directories for core_impl and pybindings
target_include_directories(core_impl PUBLIC 
                                        ${CMAKE_SOURCE_DIR}
                                        ${CMAKE_SOURCE_DIR}/cpp_code
                                        ${CMAKE_SOURCE_DIR}/cpp_code/DataObjects
                                        ${CMAKE_SOURCE_DIR}/cpp_code/Algorithm
                                        ${CMAKE_SOURCE_DIR}/cpp_code/ImageProcessing
                                        ${CMAKE_SOURCE_DIR}/cpp_code/IO
                                        ${Python3_INCLUDE_DIRS})
# target_include_directories(pybindings PRIVATE
#                                         ${CMAKE_SOURCE_DIR}
#                                         ${CMAKE_SOURCE_DIR}/cpp_code
#                                         ${CMAKE_SOURCE_DIR}/cpp_code/DataObjects
#                                         ${CMAKE_SOURCE_DIR}/cpp_code/Algorithm
#                                         ${CMAKE_SOURCE_DIR}/cpp_code/ImageProcessing
#                                         ${CMAKE_SOURCE_DIR}/cpp_code/IO
#                                         ${Python3_INCLUDE_DIRS}
# )

# Build test executable
add_executable(runTests
    cpp_code/Tests/DataObjects_Test.cpp
    cpp_code/Tests/Image_Test.cpp
    cpp_code/Tests/ImageHolderTest.cpp
    cpp_code/Tests/SnakeEngine_Test.cpp
    cpp_code/Tests/ELSnakeEngine_Test.cpp
    cpp_code/Tests/ContourResampler_Test.cpp
    cpp_code/Tests/InternalForceTest.cpp
)

# include directories for test executable (tells compiler where to find header files)
target_include_directories(runTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}          
        ${CMAKE_SOURCE_DIR}/cpp_code 
)

# link test executable with core_impl()
target_link_libraries(runTests
    PRIVATE
    gtest_main
    core_impl

)


gtest_discover_tests(runTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)