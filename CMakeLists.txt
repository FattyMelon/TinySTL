cmake_minimum_required(VERSION 3.5)
project(TinySTL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Allocator.h Alloc.h Alloc.cpp Construct.h TypeTraits.h Iterator.h Vector.h UninitializedFunctions.h Test/VectorTest.cpp Test/VectorTest.h Test/TestUtil.h)
add_executable(TinySTL ${SOURCE_FILES})