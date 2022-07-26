cmake_minimum_required(VERSION 3.21)

add_compile_definitions(WIN32_LEAN_AND_MEAN)

project(colosseum_solution)
add_subdirectory(colosseum)
add_subdirectory(forgelib)
add_subdirectory(guten)
add_subdirectory(ucilib)

set(VS_STARTUP_PROJECT colosseum)
