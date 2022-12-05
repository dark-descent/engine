# CMakeList.txt : CMake project for cpp-engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("cpp-engine")

# Add source to this project's executable.
add_executable (cpp-engine "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpp-engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
