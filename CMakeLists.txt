cmake_minimum_required(VERSION 3.3)
project(GrayCode)

find_package(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(
        ${OpenCV_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

FILE(GLOB_RECURSE GRAYCODE_SRCS "src/*")
add_executable(GrayCode ${GRAYCODE_SRCS})

target_link_libraries(GrayCode ${OpenCV_LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
