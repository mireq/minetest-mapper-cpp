project(LikesTV_API C CXX)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_RELEASE "-O2 -std=c++0x -Wall -DNDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -std=c++0x -Wall -Wextra -DDEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)


find_package(PkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_LIBSQLITE QUIET sqlite3)
set(LIBSQLITE3_DEFINITIONS ${PC_LIBSQLITE_CFLAGS_OTHER})

find_path(LIBSQLITE3_INCLUDE_DIR sqlite3.h HINTS ${PC_LIBSQLITE_INCLUDEDIR} ${PC_LIBSQLITE_INCLUDE_DIRS} PATH_SUFFIXES libsqlite3)
find_library(LIBSQLITE3_LIBRARY NAMES sqlite3 libsqlite3 HINTS ${PC_LIBSQLITE_LIBDIR} ${PC_LIBSQLITE_LIBRARY_DIRS})

set(LIBSQLITE3_LIBRARIES ${LIBSQLITE3_LIBRARY} )
set(LIBSQLITE3_INCLUDE_DIRS ${LIBSQLITE3_INCLUDE_DIR})

find_package_handle_standard_args(LibSqlite3  DEFAULT_MSG LIBSQLITE3_LIBRARY LIBSQLITE3_INCLUDE_DIR)

mark_as_advanced(LIBSQLITE3_INCLUDE_DIR LIBSQLITE3_LIBRARY)

if (LIBSQLITE3_INCLUDE_DIR)
else (LIBSQLITE3_INCLUDE_DIR)
	message(FATAL_ERROR "Could not find sqlite3")
endif (LIBSQLITE3_INCLUDE_DIR)

find_program(XXD_EXECUTABLE xxd)

if (XXD_EXECUTABLE)
	message(STATUS "Found xxd")
else (XXD_EXECUTABLE)
	message(FATAL_ERROR "Executable xxd not found")
endif (XXD_EXECUTABLE)

add_custom_command(
	OUTPUT colors.h
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/colors.txt colors.txt
	COMMAND xxd -i colors.txt colors.h
	DEPENDS colors.txt
)
set_property(SOURCE TileGenerator.cpp APPEND PROPERTY OBJECT_DEPENDS colors.h)

include_directories(
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
	${LIBSQLITE3_INCLUDE_DIRS}
)

set(mapper_HDRS
	PixelAttributes.h
	PlayerAttributes.h
	TileGenerator.h
	ZlibDecompressor.h
)

set(mapper_SRCS
	PixelAttributes.cpp
	PlayerAttributes.cpp
	TileGenerator.cpp
	ZlibDecompressor.cpp
	mapper.cpp
)

add_executable(minetest_mapper
	${mapper_HDRS}
	${mapper_SRCS}
)

target_link_libraries(
	minetest_mapper
	${LIBSQLITE3_LIBRARIES}
	gd
	z
)
