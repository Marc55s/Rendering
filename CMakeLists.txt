cmake_minimum_required(VERSION 3.15)
project(Rendering LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Raylib setup
set(RAYLIB_PATH "${CMAKE_SOURCE_DIR}/ray")  # Path to your raylib folder inside the project
set(RAYLIB_INCLUDE_DIR "${RAYLIB_PATH}/include")
set(RAYLIB_LIBRARY_DIR "${RAYLIB_PATH}/lib")

# Add include directories
include_directories(${RAYLIB_INCLUDE_DIR})

# Link libraries
link_directories(${RAYLIB_LIBRARY_DIR})

# Source files
file(GLOB SRC_FILES "*.c")  # Automatically include all .c files in the directory

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link Raylib and other required libraries
target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
