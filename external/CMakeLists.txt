if (MSVC AND NOT ${MSVC_VERSION} LESS 1400)
    if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
    add_definitions(/MP)
    endif()

    set(CMAKE_BUILD_GENERATOR Win32)
endif()

set(GLEW_SRC glew-2.1.0/src/glew.c)

add_library( GLEW_210 STATIC ${GLEW_SRC})

target_link_libraries(GLEW_210 ${OPENGL_LIBRARY} ${EXTRA_LIBS})
target_include_directories(GLEW_210 PUBLIC glew-2.1.0/include)