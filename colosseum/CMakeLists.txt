# --- Colosseum App ---

cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME colosseum)
project(${PROJECT_NAME})

set(EXAMPLES
	examples/examples.cpp
	examples/examples.h
	examples/main.cpp
)

set(GAMES
	games/games.hpp
	games/chess_match.cpp  
	games/chess_match.h  
	games/chess_tournament.cpp  
	games/chess_tournament.h
	games/round_robin.cpp
	games/round_robin.h
)

set(NOTES
	notes/high_level_design.txt
	notes/use_cases.txt
	notes/player.txt
)

set(SOURCE
	source/elo_rating.cpp
	source/elo_rating.h
	source/elo_rating_list.cpp
	source/elo_rating_list.h
	source/globals.cpp
	source/globals.h
	source/player.cpp
	source/player.h
)

set(VIEWS
	views/views.hpp
	views/no_view.cpp
	views/no_view.h
	views/text_view.cpp  
	views/text_view.h  
	views/view_base.cpp  
	views/view_base.h
)

add_executable(${PROJECT_NAME}
	${GAMES}
	${EXAMPLES}
	${NOTES}
	${SOURCE}
	${VIEWS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include_directories(${PROJECT_NAME} .)

set(VS_STARTUP_PROJECT ${PROJECT_NAME})

source_group(games FILES ${GAMES})
source_group(examples FILES ${EXAMPLES})
source_group(notes FILES ${NOTES})
source_group(source FILES ${SOURCE})
source_group(views FILES ${VIEWS})

# --- LIBRARIES ---

# --- GUTEN ---
target_link_libraries(${PROJECT_NAME} PUBLIC guten)

# --- UCILIB ---
target_link_libraries(${PROJECT_NAME} PUBLIC ucilib)

target_link_libraries(${PROJECT_NAME} PUBLIC forgelib)

# --- Preprocessor Macros ---
add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
