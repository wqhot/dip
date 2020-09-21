cmake_minimum_required(VERSION 3.8.3)
project(dip)

set(CMAKE_BUILD_TYPE "RELEASE")
# set(CMAKE_BUILD_TYPE "DEBUG")
if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++14 -O3 -pthread -fopenmp -s")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -pthread -fopenmp -s")
else(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++14 -O0 -g -Wall -pthread")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall -pthread")
endif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")

find_package(Threads)
find_package(OpenCV 3 REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/includes
        ${OpenCV_INCLUDE_DIRS}
)

add_library(diplib
        src/FreCal.cpp
        src/Image.cpp
        src/ImageBase.cpp
        src/ImagProc.cpp
        src/ImageFeature.cpp
        src/Matrix.cpp
        src/Utils.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
  diplib
  ${CMAKE_THREAD_LIBS_INIT}
  ${OpenCV_LIBS} 
)
