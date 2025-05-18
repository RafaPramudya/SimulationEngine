cmake_minimum_required(VERSION 3.16)

if (MSVC AND NOT ${MSVC_VERSION} LESS 1400)
    if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
    add_definitions(/MP)
    endif()

    set(CMAKE_BUILD_GENERATOR Win32)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring build in Debug mode")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring build in Release mode")
    if (MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    endif()
endif()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-D_CRT_SECURE_NO_WARNINGS
)

add_library(glad STATIC glad/src/glad.c glad/src/glad_wgl.c)
target_include_directories(glad PUBLIC glad/include)