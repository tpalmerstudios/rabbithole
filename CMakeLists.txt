cmake_minimum_required(VERSION 3.16)

# Rabbit Hole Description
project(RabbitHole
	VERSION
		0.0.1
	DESCRIPTION
		"A custom database system designed to store, link, and search relational data"
	LANGUAGES
		C
	)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

## Source and Header Files for Front, Backend, and Testing
# Frontend Files
file(GLOB_RECURSE RABBIT_SOURCES
	"${CMAKE_SOURCE_DIR}/rabbit/src/*.c"
)
file(GLOB_RECURSE RABBIT_HEADERS
	"${CMAKE_SOURCE_DIR}/rabbit/include/*.h"
)

# Backend Files
file(GLOB_RECURSE HOLE_SOURCES
	"${CMAKE_SOURCE_DIR}/hole/src/*.c"
)
file(GLOB_RECURSE HOLE_HEADERS
	"${CMAKE_SOURCE_DIR}/hole/include/*.h"
)

# Check & Test Files
file(GLOB_RECURSE TEST_SOURCES
	"${CMAKE_SOURCE_DIR}/tests/*.c"
)
file(GLOB_RECURSE TEST_HEADERS
	"${CMAKE_SOURCE_DIR}/tests/*.h"
)

set(ALL_C_FILES
	${RABBIT_SOURCES}
	${RABBIT_HEADERS}
	${HOLE_SOURCES}
	${HOLE_HEADERS}
	${TEST_SOURCES}
	${TEST_HEADERS}
)

# Separating the service from the library
set(HOLE_SERVICE_MAIN "${CMAKE_SOURCE_DIR}/hole/src/main.c")
set(HOLE_LIB_SOURCES ${HOLE_SOURCES})
list(REMOVE_ITEM HOLE_LIB_SOURCES ${HOLE_SERVICE_MAIN})

# Static library
add_library(hole_core
	${HOLE_LIB_SOURCES}
	${HOLE_HEADERS}
)
target_include_directories(hole_core PUBLIC
	"${CMAKE_SOURCE_DIR}/hole/include"
)
target_compile_options(hole_core PRIVATE -Wall -Wextra -Werror)

# Backend service
add_executable(hole
	${HOLE_SERVICE_MAIN}
)
target_link_libraries(hole PRIVATE hole_core)
target_include_directories(hole PRIVATE
	"${CMAKE_SOURCE_DIR}/hole/include"
)
target_compile_options(hole PRIVATE -Wall -Wextra -Werror)

# Frontend
add_executable(rabbit
	${RABBIT_SOURCES}
	${RABBIT_HEADERS}
)
target_include_directories(rabbit PRIVATE
	${RABBIT_HEADERS}
)
target_link_libraries(rabbit PRIVATE hole_core)
target_compile_options(rabbit PRIVATE -Wall -Wextra -Werror)


# clang-format target
# Esures proper formatting of all code

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE AND ALL_C_FILES)
	add_custom_target(
		clang-format
		COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_C_FILES}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Running clang-format on source files"
	)
else()
	message(STATUS "clang-format not found, 'clang-format' target will be skipped")
endif()

# doxygen target
# Ensures documentation is created on all code
find_package(Doxygen)

if(DOXYGEN_FOUND)
	add_custom_target(
		docs
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating Documentation with Dxygen"
		VERBATIM
	)
else()
	message(STATUS "Doxygen not found, 'docs' target willbe skipped")
endif()
# check target
# Ensures testing is performed
enable_testing()

if(TEST_SOURCES)
	add_executable(test_runner
		${TEST_SOURCES}
		${TEST_HEADERS}
	)

	target_include_directories(test_runner PRIVATE
		"${CMAKE_SOURCE_DIR}/rabbit/include"
		"${CMAKE_SOURCE_DIR}/hole/include"
	)

	target_link_libraries(test_runner PRIVATE hole_core)

	target_compile_options(test_runner PRIVATE
		-Wall -Wextra -Werror
	)

	add_test(NAME unit_tests COMMAND test_runner)

	add_custom_target(
		check
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
		DEPENDS test_runner
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Running unit tests (ctest)"
	)
else()
	message(STATUS "No tests found in tests/*.c. 'check' target will be skipped")
endif()

# rabbithole target
# Runs all targets
add_custom_target(rabbithole ALL
	COMMENT "Build all targets: Rabbit, Hole, formatting, documentation, testing"
)

add_dependencies(rabbithole rabbit hole)

if(TARGET clang-format)
	add_dependencies(rabbithole clang-format)
endif()

if(TARGET docs)
	add_dependencies(rabbithole docs)
endif()

if(TARGET check)
	add_dependencies(rabbithole check)
endif()

