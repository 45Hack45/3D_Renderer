cmake_minimum_required(VERSION 3.0.0)
project(3D_Renderer VERSION 0.1.0 LANGUAGES C CXX)

message(STATUS "Generating Engine")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
set(ENGINE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ENGINE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CXX_STANDARD c++17)
add_subdirectory(src)
message(STATUS "Finished Generating Engine")