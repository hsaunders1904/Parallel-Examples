cmake_minimum_required(VERSION 3.5)
project(MPI VERSION 0.0.1)

find_package(MPI REQUIRED)
message("Found MPI, with mpiexec=${MPIEXEC}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable("Hello-World" MPIHelloWorld.cpp)
target_include_directories("Hello-World" PUBLIC ${MPI_CXX_INCLUDE_PATH})
target_link_libraries("Hello-World" PUBLIC ${MPI_CXX_LIBRARIES})