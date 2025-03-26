cmake_minimum_required(VERSION 3.16)

if (MSVC AND NOT ${MSVC_VERSION} LESS 1400)
    if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
    add_definitions(/MP)
    endif()

    set(CMAKE_BUILD_GENERATOR Win32)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CONFIGURATION}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CONFIGURATION}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-D_CRT_SECURE_NO_WARNINGS
)

project(Engine VERSION 0.0.1)

file(GLOB_RECURSE SRC_FILE src/*.c src/*.cpp)

set(EXECUTABLE_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_NAME} ${SRC_FILE})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_17)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_23)