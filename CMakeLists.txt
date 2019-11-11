cmake_minimum_required(VERSION 3.5)
project(MPI VERSION 0.0.1)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
if(WIN32)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

# Hello-World executable
add_executable("MPI-Hello-World" MPI-HelloWorld.cpp)
target_link_libraries("MPI-Hello-World" PRIVATE MPI::MPI_CXX)

# Send-Receive executable
add_executable("MPI-Send-Receive" MPI-SendReceive.cpp)
target_link_libraries("MPI-Send-Receive" PRIVATE MPI::MPI_CXX)

# OpenMP Pi calculation executable
add_executable("OpenMP-Pi" OpenMP-Pi.cpp)
target_link_libraries("OpenMP-Pi" PRIVATE OpenMP::OpenMP_CXX)

# OpenMP Pi Reduce executable
add_executable("OpenMP-Reduce" OpenMP-Reduce.cpp)
target_link_libraries("OpenMP-Reduce" PRIVATE OpenMP::OpenMP_CXX)

# OpenMP tasks Fibonacci example
# OpenMP task directives only available on v3.0 and upward
if(OpenMP::OpenMP_VERSION GREATER_EQUAL 3)
  add_executable("OpenMP-Fibonacci" OpenMP-Fibonacci.cpp)
  target_link_libraries("OpenMP-Fibonacci" PRIVATE OpenMP::OpenMP_CXX)
endif()

# OpenMPI MPI_Get_Count example
add_executable("MPI-Get-Count" MPI-GetCount)
target_link_libraries("MPI-Get-Count" PRIVATE MPI::MPI_CXX)

# OpenMPI MPI_Probe example
add_executable("MPI-Probe" MPI-GetCount)
target_link_libraries("MPI-Probe" PRIVATE MPI::MPI_CXX)

# Add MPI random walk example
add_executable("MPI-Random-Walk" MPI-RandomWalk)
target_link_libraries("MPI-Random-Walk" PRIVATE MPI::MPI_CXX)
