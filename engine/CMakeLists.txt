project(nexus-engine VERSION 1.0)

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

# Tests
if(PROJECT_NAME STREQUAL "nexus-engine")
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
    add_executable(nexus-engine-tests ${TEST_SOURCES})

    target_link_libraries(nexus-engine-tests PRIVATE nexus-engine)
    target_include_directories(nexus-engine-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_test(NAME nexus-engine-tests COMMAND nexus-engine-tests)
endif()
