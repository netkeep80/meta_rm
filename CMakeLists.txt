cmake_minimum_required(VERSION 3.20)
project(meta_rm LANGUAGES CXX)

add_library(meta_rm INTERFACE)
target_include_directories(meta_rm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(meta_rm INTERFACE cxx_std_20)

add_executable(meta_rm_example examples/inherit_chain.cpp)
target_link_libraries(meta_rm_example PRIVATE meta_rm)

add_executable(meta_rm_smoke tests/smoke.cpp)
target_link_libraries(meta_rm_smoke PRIVATE meta_rm)
