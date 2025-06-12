cmake_minimum_required(VERSION 3.16)

project(dtk_kernel LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files explicitly
add_executable(dtk_kernel_app
    src/main.cpp
    src/dtk_kernel.cpp
    src/dtk_task_handler.cpp
)

# Optional: Add include dirs this way (better scoping)
target_include_directories(dtk_kernel_app PRIVATE include)

# Optional: treat warnings as errors (good for dev)
target_compile_options(dtk_kernel_app PRIVATE -Wall -Wextra -pedantic -Werror)
