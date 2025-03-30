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
	-DTW_NO_DIRECT3D
    -DGLEW_STATIC
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(
        DEBUG
    )
endif()

project(Engine VERSION 0.0.1)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(SDL3 EXCLUDE_FROM_ALL)

set(EXECUTABLE_NAME ${PROJECT_NAME})
file(GLOB_RECURSE SRC_FILE src/*.c src/*.cpp)
add_executable(${EXECUTABLE_NAME} ${SRC_FILE})

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    include
    external/glew-2.1.0/include
)
target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_99)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_23)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC SDL3::SDL3 ${OPENGL_LIBRARY} GLEW_210)