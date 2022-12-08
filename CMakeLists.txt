cmake_minimum_required (VERSION 3.8)

project (dark-descent-engine)

file(GLOB dark-descent-srcs CONFIGURE_DEPENDS "include/*.h" "src/*.cpp")

add_executable (dark-descent-engine WIN32 ${dark-descent-srcs})

set(SDL2_DIR "dependencies/win/SDL2/cmake")

find_package(SDL2 REQUIRED)

target_precompile_headers(dark-descent-engine PUBLIC include/pch.hpp)

include_directories(dark-descent-engine ${SDL2_INCLUDE_DIRS} include)

target_link_libraries(dark-descent-engine ${SDL2_LIBRARIES} opengl32)

# add_definitions(-DSDL_MAIN_HANDLED)

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET dark-descent-engine PROPERTY CXX_STANDARD 23)
endif()


if (WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(SDL_DLL "${PROJECT_SOURCE_DIR}/dependencies/win/SDL2/lib/x64/SDL2.dll")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(SDL_DLL "${PROJECT_SOURCE_DIR}/dependencies/win/SDL2/lib/x86/SDL2.dll")
	endif()
endif (WIN32)

add_custom_command(TARGET dark-descent-engine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_DLL} $<TARGET_FILE_DIR:dark-descent-engine>)
