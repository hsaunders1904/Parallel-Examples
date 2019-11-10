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
add_executable("Hello-World" MPIHelloWorld.cpp)
target_link_libraries("Hello-World" PRIVATE MPI::MPI_CXX)

# Send-Receive executable
add_executable("Send-Receive" MPISendReceive.cpp)
target_link_libraries("Send-Receive" PRIVATE MPI::MPI_CXX)

# OpenMP Pi calculation executable
add_executable("OpenMP-Pi" OpenMPPi.cpp)
target_link_libraries("OpenMP-Pi" PRIVATE OpenMP::OpenMP_CXX)

# OpenMP Pi Reduce executable
add_executable("OpenMP-Reduce" OpenMPReduce.cpp)
target_link_libraries("OpenMP-Reduce" PRIVATE OpenMP::OpenMP_CXX)

# OpenMP tasks Fibonacci example
# OpenMP task directives only available on v3.0 and upward
if(OpenMP::OpenMP_VERSION GREATER_EQUAL 3)
  add_executable("Fibonacci-OpenMP" fibonacci.cpp)
  target_link_libraries("Fibonacci-OpenMP" PRIVATE OpenMP::OpenMP_CXX)
endif()

# OpenMPI MPI_Get_Count example
add_executable("MPI-Get-Count" MPIGetCount)
target_link_libraries("MPI-Get-Count" PRIVATE MPI::MPI_CXX)

# OpenMPI MPI_Probe example
add_executable("MPI-Probe" MPIGetCount)
target_link_libraries("MPI-Probe" PRIVATE MPI::MPI_CXX)
