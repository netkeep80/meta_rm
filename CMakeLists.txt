cmake_minimum_required(VERSION 3.20)
project(meta_rm LANGUAGES CXX)

option(META_RM_BUILD_TESTS "Build meta_rm tests" ON)
option(META_RM_BUILD_EXAMPLES "Build meta_rm examples" ON)

add_library(meta_rm INTERFACE)
target_include_directories(meta_rm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(meta_rm INTERFACE cxx_std_20)

if(META_RM_BUILD_EXAMPLES)
    add_executable(meta_rm_example examples/inherit_chain.cpp)
    target_link_libraries(meta_rm_example PRIVATE meta_rm)
endif()

if(META_RM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
