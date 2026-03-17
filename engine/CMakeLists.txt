project(nexus-engine VERSION 1.0)

include(FetchContent)

# Sources
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Create library
add_library(nexus-engine STATIC ${ENGINE_SOURCES})

# C++ Standard
target_compile_features(nexus-engine PUBLIC cxx_std_23)

# Compiler Flags
target_compile_options(nexus-engine PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

# Include directories
target_include_directories(nexus-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

target_link_libraries(nexus-engine PUBLIC glfw)

# Tests
if(PROJECT_NAME STREQUAL "nexus-engine")
    # Google Test
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
    add_executable(nexus-engine-tests ${TEST_SOURCES})
    target_link_libraries(nexus-engine-tests PRIVATE nexus-engine gtest_main)
    target_include_directories(nexus-engine-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    include(GoogleTest)
    gtest_discover_tests(nexus-engine-tests)
endif()