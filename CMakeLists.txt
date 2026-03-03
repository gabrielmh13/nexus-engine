cmake_minimum_required(VERSION 3.20)
project(NexusEngineProject VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(engine)
add_subdirectory(game)
