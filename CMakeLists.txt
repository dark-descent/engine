# CMakeList.txt : CMake project for dark-descent-engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("dark-descent-engine")

# Add source to this project's executable.
add_executable (dark-descent-engine "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dark-descent-engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
