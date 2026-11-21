cmake_minimum_required(VERSION 3.21)
project(XBridgeSeries VERSION 0.3.0 LANGUAGES CXX)

option(XBRIDGE_BUILD_TESTS "Build XBridge tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(xbridge_core
    src/core/config.cpp
    src/core/runtime.cpp
    src/cpu/test_cpu.cpp
    src/memory/memory.cpp
    src/profiles/hardware_profile.cpp
    src/host/execution_policy.cpp
)
target_include_directories(xbridge_core PUBLIC include)
target_compile_features(xbridge_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(xbridge_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(xbridge_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(xbridge src/app/main.cpp)
target_link_libraries(xbridge PRIVATE xbridge_core)

if(XBRIDGE_BUILD_TESTS)
    enable_testing()
    add_executable(xbridge_tests tests/test_main.cpp)
    target_link_libraries(xbridge_tests PRIVATE xbridge_core)
    add_test(NAME xbridge_tests COMMAND xbridge_tests)
endif()
