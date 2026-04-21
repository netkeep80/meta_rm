cmake_minimum_required(VERSION 3.20)
project(meta_rm LANGUAGES CXX)

option(META_RM_BUILD_TESTS "Build meta_rm tests" ON)
option(META_RM_BUILD_EXAMPLES "Build meta_rm examples" ON)
option(META_RM_ENABLE_WARNINGS "Enable strict warnings for meta_rm targets" OFF)

add_library(meta_rm INTERFACE)
target_include_directories(meta_rm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(meta_rm INTERFACE cxx_std_20)

# Shared warning flags applied to first-party targets only. Catch2 and other
# fetched dependencies are not affected because they are linked as system-style
# libraries through their own targets.
add_library(meta_rm_warnings INTERFACE)
if(META_RM_ENABLE_WARNINGS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(meta_rm_warnings INTERFACE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Werror
        )
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(meta_rm_warnings INTERFACE /W4 /WX)
    endif()
endif()

if(META_RM_BUILD_EXAMPLES)
    add_executable(meta_rm_example examples/inherit_chain.cpp)
    target_link_libraries(meta_rm_example PRIVATE meta_rm meta_rm_warnings)
endif()

if(META_RM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
