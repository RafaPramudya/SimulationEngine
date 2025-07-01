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

project(Engine VERSION 0.0.1)

find_package(OpenGL REQUIRED)

# add_library(Assimp STATIC IMPORTED)
# set_target_properties(Assimp PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/assimp-vc143-mt.lib")

# include(FetchContent)
# set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)
# FetchContent_Declare(assimp
# GIT_REPOSITORY https://github.com/assimp/assimp.git
# GIT_TAG master)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(assimp)

add_subdirectory(assimp)
add_subdirectory(glm)

# Ensure the main.c file is included in the SRC_FILE list
file(GLOB_RECURSE SRC_FILE CONFIGURE_DEPENDS 
    core/*.cpp
    core/windows/*.cpp
    core/utils/*.cpp
    core/render/*.cpp
    core/component/*.cpp
    core/main.cpp  # Explicitly include main.c
)

set(CGLM_STATIC ON)

# Set the linker subsystem to WINDOWS for Windows native applications
if (WIN32 AND MSVC)
    set(EXECUTABLE_NAME ${PROJECT_NAME})
    add_executable(${EXECUTABLE_NAME} ${SRC_FILE}
        lib/glad/src/glad.c
        lib/glad/src/glad_wgl.c
    )
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
else()
    # ERROR: Unsupported platform
    message(FATAL_ERROR "Unsupported platform")
endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC
    core
    lib/glad/include
    libs/assimp-src/include
    lib
)
target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_17)
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_23)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenGL::GL glm::glm assimp)

# add_custom_command(
#     TARGET Engine POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/assimp-vc143-mt.dll"
#     "$<TARGET_FILE_DIR:Engine>"
# )