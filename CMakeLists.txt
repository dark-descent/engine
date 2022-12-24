cmake_minimum_required (VERSION 3.8)

project (dark-descent-engine)

file(GLOB dark-descent-srcs CONFIGURE_DEPENDS "include/*.h" "src/*.cpp" "include/js/*.h" "src/js/*.cpp")

add_executable (dark-descent-engine WIN32 ${dark-descent-srcs})

set(SDL2_DIR "dependencies/win/SDL2/cmake")

find_package(SDL2 REQUIRED)

target_precompile_headers(dark-descent-engine PUBLIC include/pch.hpp)

include_directories(dark-descent-engine ${SDL2_INCLUDE_DIRS} include)

target_link_libraries(dark-descent-engine ${SDL2_LIBRARIES} opengl32)

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
	include_directories(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/include)
	include_directories(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/glew/include/GL)

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    	set(V8_TYPE "debug")
		set(GLEW_TYPE, "Debug")
	else()
		set(V8_TYPE "release")
		set(GLEW_TYPE, "Release")
	endif()

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0 -DV8_COMPRESS_POINTERS -DGLEW_STATIC)

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/glew/lib/Release/x64/glew32s.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x64.${V8_TYPE}/obj/v8_monolith.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x64.${V8_TYPE}/obj/v8_libbase.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x64.${V8_TYPE}/obj/v8_libplatform.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x64.${V8_TYPE}/obj/v8_snapshot.lib)
		set(SDL_DLL "${PROJECT_SOURCE_DIR}/dependencies/win/SDL2/lib/x64/SDL2.dll")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/glew/lib/Release/Win32/glew32s.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x86.${V8_TYPE}/obj/v8_monolith.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x86.${V8_TYPE}/obj/v8_libbase.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x86.${V8_TYPE}/obj/v8_libplatform.lib)
		target_link_libraries(dark-descent-engine ${PROJECT_SOURCE_DIR}/dependencies/win/v8/x86.${V8_TYPE}/obj/v8_snapshot.lib)
		set(SDL_DLL "${PROJECT_SOURCE_DIR}/dependencies/win/SDL2/lib/x86/SDL2.dll")
	endif()
	add_custom_command(TARGET dark-descent-engine POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_DLL} $<TARGET_FILE_DIR:dark-descent-engine>)
endif (WIN32)

