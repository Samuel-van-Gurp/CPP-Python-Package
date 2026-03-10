cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
project(C_API LANGUAGES C CXX)

find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)

if(DEFINED ENV{SETUPTOOLS_EXT_SUFFIX})
    set(CMAKE_SHARED_MODULE_SUFFIX $ENV{SETUPTOOLS_EXT_SUFFIX})
endif()


if(DEFINED ENV{SETUPTOOLS_EXT_SUFFIX})
    set_target_properties(pybindings PROPERTIES SUFFIX $ENV{SETUPTOOLS_EXT_SUFFIX})
endif()


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
# Disable GoogleTest installation to prevent static libraries from being included in the Python wheel
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest) 

# STATIC library with core implementation
add_library(core_impl STATIC
    MagicNumbers.hpp
    SnakeController.cpp

    DataObjects/Contour.cpp
    DataObjects/SnakeParams.hpp
    Algorithm/ContourResampler.cpp
    Algorithm/ELSnakeEngine.cpp
    Algorithm/GreedySnakeEngine.cpp
    Algorithm/ISnakeEngine.cpp

    ImageProcessing/ImageProcessorFacade.cpp
    ImageProcessing/NaiveConvolve.cpp
    ImageProcessing/IntensityManipulator.cpp
)
set_target_properties(core_impl PROPERTIES POSITION_INDEPENDENT_CODE ON) # for linux build, necessary for shared libs

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(core_impl PRIVATE --coverage -O0 -g)
    target_link_options(core_impl PRIVATE --coverage)
endif()

pybind11_add_module(pybindings bindings.cpp)  # create pybindings module

#  Link core_impl and pybindings
target_link_libraries(pybindings PRIVATE core_impl)

# Include directories for core_impl and pybindings
## Include directories (use CMAKE_CURRENT_SOURCE_DIR so this subproject is relocatable)
target_include_directories(core_impl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/DataObjects
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/ImageProcessing
    ${CMAKE_CURRENT_SOURCE_DIR}/IO
    ${Python3_INCLUDE_DIRS}
)

# only build tests if not building awheel with scikit-build 
if(NOT SKBUILD)
    # Build test executable
    add_executable(runTests
        Tests/DataObjects_Test.cpp
        Tests/Image_Test.cpp
        Tests/ImageHolderTest.cpp
        Tests/SnakeEngine_Test.cpp
        Tests/ELSnakeEngine_Test.cpp
        Tests/ContourResampler_Test.cpp
        Tests/InternalForceTest.cpp
        Tests/GreedySnakeTest.cpp

        Tests/Test_Classes/GreedySnakeTestWrapper.cpp
    )

    # include directories for test executable (tells compiler where to find header files)
    target_include_directories(runTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/DataObjects
    )

    # link test and mock executables with core_impl
    target_link_libraries(runTests
        PRIVATE
        gtest_main
        gmock_main
        core_impl
    )

    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(runTests PRIVATE --coverage -O0 -g)
        target_link_options(runTests PRIVATE --coverage)
    endif()

    gtest_discover_tests(runTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

# install the pybindings module to the SnakesPy package directory
install(TARGETS pybindings DESTINATION SnakesPy)