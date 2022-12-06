cmake_minimum_required (VERSION 3.8)

project (dark-descent-engine)
if (WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(dep_libs "dependencies/win/lib/x64")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(dep_libs "dependencies/win/lib/x86")
	endif()
	
	add_definitions(-DUNICODE -D_UNICODE)
	set(sdl_includes "dependencies/win/sdl/include")
endif (WIN32)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${sdl_includes})
link_directories(${dep_libs})

include_directories(dark-descent-engine "include")

add_executable (dark-descent-engine WIN32 "src/main.cpp" "src/Logger.cpp")

target_link_libraries(dark-descent-engine SDL2 SDL2main opengl32)

target_precompile_headers(dark-descent-engine PUBLIC "include/pch.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dark-descent-engine PROPERTY CXX_STANDARD 20)
endif()
