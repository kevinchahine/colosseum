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

# --- Libraries (VCPKG) ---
if (MSVC)
	message(STATUS "\nVCPKG packages will be automatically linked to visual studio\n")
else ()
	# --- For VCPKG packages ---
	#message(STATUS "\nCall cmake with: cmake .. -DCMAKE_TOOLCHAIN_FILE=/__path__/__to__/vcpkg/scripts/buildsystems/vcpkg.cmake")

	# ----- BOOST -----

	message(STATUS "\n----- BOOST -----")
	set(Boost_USE_STATIC_LIBS		OFF)
	set(Boost_USE_MULTITHREADED		OFF)
	set(Boost_USE_STATIC_RUNTIME	OFF)
	set(DCMAKE_TOOLCHAIN_FILE="/media/kevin/barracuda/Code/libraries/vcpkg/scripts/buildsystems/vcpkg.cmake")
	set(DCMAKE_PREFIX_PATH="/media/kevin/barracuda/Code/libraries/libtorch/libtorch-cxx11-abi-shared-with-deps-1.11.0+cu113/libtorch")

	find_package(Boost REQUIRED COMPONENTS
		#algorithm		# leave out header-only packages
		#asio 			# leave out header-only packages
		#process		# leave out header-only packages
		system 
		filesystem 
		regex
		thread
	)

	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})

	target_link_libraries(${PROJECT_NAME} PUBLIC
		#Boost::algorithm
		#Boost::asio
		Boost::system
		Boost::filesystem
		Boost::regex
		${Boost_LIBRARIES}	# TODO: do we need this
	)

	message(STATUS "Include dirs ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost libraries ${Boost_LIBRARIES}")

	target_include_directories(${PROJECT_NAME} PUBLIC
		${Boost_INCLUDE_DIRS}
	)
endif (MSVC)

# --- UCILIB ---

add_subdirectory(uci)

#target_link_libraries(${PROJECT_NAME} PUBLIC uci)
target_link_libraries(${PROJECT_NAME} PUBLIC ucilib)
