project(LikesTV_API C CXX)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(PkgConfig)
pkg_check_modules(PC_LIBSQLITE QUIET sqlite3)
set(LIBSQLITE3_DEFINITIONS ${PC_LIBSQLITE_CFLAGS_OTHER})

find_path(LIBSQLITE3_INCLUDE_DIR sqlite3.h HINTS ${PC_LIBSQLITE_INCLUDEDIR} ${PC_LIBSQLITE_INCLUDE_DIRS} PATH_SUFFIXES libsqlite3)
find_library(LIBSQLITE3_LIBRARY NAMES sqlite3 libsqlite3 HINTS ${PC_LIBSQLITE_LIBDIR} ${PC_LIBSQLITE_LIBRARY_DIRS})

set(LIBSQLITE3_LIBRARIES ${LIBSQLITE3_LIBRARY} )
set(LIBSQLITE3_INCLUDE_DIRS ${LIBSQLITE3_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibSqlite3  DEFAULT_MSG LIBSQLITE3_LIBRARY LIBSQLITE3_INCLUDE_DIR)

mark_as_advanced(LIBSQLITE3_INCLUDE_DIR LIBSQLITE3_LIBRARY )



include_directories(
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
	${LIBSQLITE3_INCLUDE_DIRS}
)

set(mapper_HDRS
)

set(mapper_SRCS
	mapper.cpp
)

add_executable(minetest_mapper
	${mapper_HDRS}
	${mapper_SRCS}
)

target_link_libraries(
	minetest_mapper
	${LIBSQLITE3_LIBRARIES}
)
