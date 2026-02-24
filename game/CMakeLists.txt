project(nexus-game VERSION 1.0)

# Sources
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Create executable
add_executable(nexus-game ${GAME_SOURCES})

# C++ Standard
target_compile_features(nexus-game PRIVATE cxx_std_23)

# Compiler Flags
target_compile_options(nexus-game PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

# Link engine library
target_link_libraries(nexus-game PRIVATE nexus-engine)
