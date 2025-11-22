cmake_minimum_required(VERSION 3.16)
project(MathParser LANGUAGES CXX)

# Usar C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir carpeta include/
include_directories(include)

# Buscar todos los .cpp del proyecto
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "main.cpp"
)

# Crear ejecutable
add_executable(test ${SOURCES})

