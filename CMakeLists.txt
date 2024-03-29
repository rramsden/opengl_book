project(opengl_macos)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glew 2.1.0 REQUIRED)

set(LIBRARIES GLEW glfw ${OPENGL_LIBRARY})

file (GLOB HEADERS *.h )
include_directories(${CMAKE_SOURCE_DIR})

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(chapter_1 chapter_1.cpp)
target_link_libraries(chapter_1 ${LIBRARIES})

add_executable(chapter_2.1 chapter_2.1.cpp)
target_link_libraries(chapter_2.1 ${LIBRARIES})

add_executable(chapter_2.2 chapter_2.2.cpp)
target_link_libraries(chapter_2.2 ${LIBRARIES})

add_executable(chapter_2.3 chapter_2.3.cpp)
target_link_libraries(chapter_2.3 ${LIBRARIES})

add_executable(chapter_2.4 chapter_2.4.cpp)
target_link_libraries(chapter_2.4 ${LIBRARIES})

add_executable(chapter_3 chapter_3.cpp)
target_link_libraries(chapter_3 ${LIBRARIES})

add_executable(chapter_4 chapter_4.cpp Utils.cpp ${HEADERS})
target_link_libraries(chapter_4 ${LIBRARIES})