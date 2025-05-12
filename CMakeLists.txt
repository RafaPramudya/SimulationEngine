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

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
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

project(Engine VERSION 0.0.1)

# Ensure the main.c file is included in the SRC_FILE list
file(GLOB_RECURSE SRC_FILE CONFIGURE_DEPENDS 
    core/*.c
    core/*.cpp
    core/windows/*.c
    core/windows/*.cpp
    core/main.c  # Explicitly include main.c
)

# Set the linker subsystem to WINDOWS for Windows native applications
if (WIN32 AND MSVC)
    set(EXECUTABLE_NAME ${PROJECT_NAME})
    add_executable(${EXECUTABLE_NAME} ${SRC_FILE})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
else()
    # ERROR: Unsupported platform
    message(FATAL_ERROR "Unsupported platform")
endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC
    core
    core/windows
)
target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_17)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_23)