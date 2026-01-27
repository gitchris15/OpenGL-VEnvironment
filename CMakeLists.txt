cmake_minimum_required(VERSION 3.10)
project(PracticasOpenGL VERSION 1.0 LANGUAGES C CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de inclusión
include_directories(include)

# Buscar GLFW3
find_package(glfw3 CONFIG REQUIRED)

# Fuentes
set(SOURCES
    src/main.cpp
    src/glad.c
)

# Ejecutable
add_executable(app ${SOURCES})

# Vincular librerías
target_link_libraries(app glfw)

# Configuración específica para macOS
if(APPLE)
    target_link_libraries(app "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# Configuración específica para Windows (MSVC)
if(MSVC)
    target_link_libraries(app opengl32.lib)
endif()
