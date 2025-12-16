cmake_minimum_required(VERSION 3.10)

project(vacation_calculator C)

# Set C standard to C99 for portability
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform-specific configurations
if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Enable warnings for better code quality
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Main executable
add_executable(vacation_calculator src/main.c)

# Test executable
add_executable(test_vacation src/test_vacation.c src/main.c)

# Link math library on Unix systems
if(UNIX)
    target_link_libraries(vacation_calculator m)
    target_link_libraries(test_vacation m)
endif()

# Define TEST_BUILD to exclude main() from main.c when building tests
target_compile_definitions(test_vacation PRIVATE TEST_BUILD)

# Enable testing
enable_testing()
add_test(NAME vacation_tests COMMAND test_vacation)
