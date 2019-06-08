cmake_minimum_required(VERSION 3.14)
project(lab12)

set(CMAKE_CXX_STANDARD 17)

#MPI
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAG})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})

add_executable(mpi_casablanca mpi_casablanca.cpp)
target_link_libraries(mpi_casablanca ${MPI_LIBRARIES})

add_executable(sequential_casablanca sequential_casablanca.cpp)
target_link_libraries(sequential_casablanca ${MPI_LIBRARIES})

add_executable(sequential_boulevard sequential_boulevard.cpp)
target_link_libraries(sequential_boulevard ${MPI_LIBRARIES})

add_executable(mpi_boulevard mpi_boulevard.cpp)
target_link_libraries(mpi_boulevard ${MPI_LIBRARIES})
