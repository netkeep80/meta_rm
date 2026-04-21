include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.3
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(meta_rm_tests
    smoke.cpp
    relation_inherit.cpp
    relation_root.cpp
    primitives.cpp
)
target_link_libraries(meta_rm_tests PRIVATE meta_rm meta_rm_warnings Catch2::Catch2WithMain)
target_compile_features(meta_rm_tests PRIVATE cxx_std_20)

catch_discover_tests(meta_rm_tests)
