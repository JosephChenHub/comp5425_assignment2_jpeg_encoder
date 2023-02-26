cmake_minimum_required(VERSION 3.13)
project(jpeg)

set(EXE  jpeg_encoder) # exe. name

set(CMAKE_CXX_STANDARD 14) # c++14

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Set the optimization level to O0
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Option to build Python module (default: OFF)
option(BUILD_PYTHON_MODULE "Build Python module" OFF)
option(BUILD_TESTS "Build tests" OFF)




# Add include directories
include_directories(include)

if(BUILD_PYTHON_MODULE)
    add_subdirectory(pybind11)
    #find_package(pybind11 REQUIRED)
    
    pybind11_add_module(jpeg_py MODULE python/bind.cpp 
                        src/HuffmanCodec.cpp  
                        src/JpegIO.cpp
                        src/JpegZigzag.cpp
                        #src/JpegDCT.cpp
                        src/JpegColor.cpp
                        src/image.cpp
                        3rdparty/bitstr.cpp
                       )
endif()

if (BUILD_TESTS)
    # Add the gtest directory to your include paths
    add_subdirectory(googletest)
    # Include the gtest library in your target
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    # Add your target executable
    add_executable(test_image test/test_image.cpp src/image.cpp)

    # Link against the gtest library and any other necessary libraries
    target_link_libraries(test_image gtest_main pthread)    
endif()

add_executable(${EXE} 
        src/encoder.cpp
        src/JpegEncoder.cpp 
        #src/JpegDCT.cpp 
        src/JpegQuant.cpp 
        src/JpegZigzag.cpp 
        src/HuffmanCodec.cpp
	src/JpegIO.cpp
        src/JpegColor.cpp
        src/image.cpp
        3rdparty/bitstr.cpp
        )


