project(opengl_macos)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(LIBRARIES glfw ${OPENGL_LIBRARY})

add_executable(chapter_1 chapter_1.cpp)
target_link_libraries(chapter_1 ${LIBRARIES})
