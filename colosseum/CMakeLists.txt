cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME colosseum)
project(${PROJECT_NAME})

set(EXAMPLES
	examples/engine_vs_engine.cpp
	examples/examples.h
	examples/main.cpp
)

set(NOTES
	notes/high_level_design.txt
)

add_executable(${PROJECT_NAME}
	${EXAMPLES}
	${NOTES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include_directories(${PROJECT_NAME} .)

set(VS_STARTUP_PROJECT ${PROJECT_NAME})

source_group(examples FILES ${EXAMPLES})
source_group(notes FILES ${NOTES})

# --- LIBRARIES ---

# --- GUTEN ---
target_link_libraries(${PROJECT_NAME} PUBLIC guten)
# target_include_directories(${PROJECT_NAME} PUBLIC ../Guten/)

# --- UCILIB ---
target_link_libraries(${PROJECT_NAME} PUBLIC ucilib)
# target_include_directories(${PROJECT_NAME} PUBLIC ucilib)

