cmake_minimum_required(VERSION 3.14)
project(lab11)

set(CMAKE_CXX_STANDARD 17)

#MPI
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAG})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})


add_executable(zad1 "zad1.cpp")
target_link_libraries(zad1 ${MPI_LIBRARIES})
add_executable(zad2 "zad2.cpp")
target_link_libraries(zad2 ${MPI_LIBRARIES})