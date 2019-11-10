cmake_minimum_required(VERSION 3.5)
project(MPI VERSION 0.0.1)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
message(STATUS "Found MPI, with mpiexec=${MPIEXEC}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
if(WIN32)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()


# Hello-World executable
add_executable("Hello-World" MPIHelloWorld.cpp)
target_include_directories("Hello-World" PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries("Hello-World" PUBLIC ${MPI_CXX_LIBRARIES})

# Send-Receive executable
add_executable("Send-Receive" MPISendReceive.cpp)
target_include_directories("Send-Receive" PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries("Send-Receive" PUBLIC ${MPI_CXX_LIBRARIES})

# OpenMP Pi calculation executable
add_executable("OpenMP-Pi" OpenMPPi.cpp)
target_link_libraries("OpenMP-Pi" PUBLIC ${OpenMP_CXX_LIBRARIES})
target_compile_options("OpenMP-Pi" PUBLIC ${OpenMP_CXX_FLAGS})

# OpenMP Pi Reduce executable
add_executable("OpenMP-Reduce" OpenMPReduce.cpp)
target_link_libraries("OpenMP-Reduce" PUBLIC ${OpenMP_CXX_LIBRARIES})
target_compile_options("OpenMP-Reduce" PUBLIC ${OpenMP_CXX_FLAGS})

# Only compile this on UNIX, since tasks are only supported on OpenMP > v3.0.
# Windows only supports v2.0.
if(OpenMP::OpenMP_VERSION GREATER_EQUAL 3)
  message(STATUS "OpenMP version: " OpenMP::OpenMP_VERSION)
  # OpenMP tasks Fibonacci example
  add_executable("Fibonacci-OpenMP" fibonacci.cpp)
  target_link_libraries("Fibonacci-OpenMP" PUBLIC ${OpenMP_CXX_LIBRARIES})
  target_compile_options("Fibonacci-OpenMP" PUBLIC ${OpenMP_CXX_FLAGS})
endif()

# OpenMPI MPI_Get_Count example
add_executable("MPI-Get-Count" MPIGetCount)
target_include_directories("MPI-Get-Count" PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries("MPI-Get-Count" PUBLIC ${MPI_CXX_LIBRARIES})

# OpenMPI MPI_Probe example
add_executable("MPI-Probe" MPIGetCount)
target_include_directories("MPI-Probe" PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries("MPI-Probe" PUBLIC ${MPI_CXX_LIBRARIES})
