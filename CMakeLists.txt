cmake_minimum_required(VERSION 3.10)
project(PokemonBattle)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${Python3_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/Pokemon.cpp
    src/Move.cpp
    src/Battle.cpp
    src/PythonSkillLoader.cpp
    src/TypeEffectiveness.cpp
    src/main.cpp
)

# Create executable
add_executable(pokemon_battle ${SOURCES})

# Link Python libraries
target_link_libraries(pokemon_battle ${Python3_LIBRARIES})

# Installation
install(TARGETS pokemon_battle DESTINATION bin)

# Copy Python scripts to build directory
file(COPY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${CMAKE_BINARY_DIR})
