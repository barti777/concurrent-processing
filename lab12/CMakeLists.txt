cmake_minimum_required(VERSION 3.14)
project(lab12)

set(CMAKE_CXX_STANDARD 17)

#MPI
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAG})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})

add_executable(zadanie zadanie.cpp)
target_link_libraries(zadanie ${MPI_LIBRARIES})
add_executable(zadanie_structural_casablanca zadanie_structural_casablanca.cpp)
target_link_libraries(zadanie_structural_casablanca ${MPI_LIBRARIES})
add_executable(zadanie_structural_boulevard zadanie_structural_boulevard.cpp)
target_link_libraries(zadanie_structural_boulevard ${MPI_LIBRARIES})