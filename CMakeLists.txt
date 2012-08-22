project(LikesTV_API C CXX)
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


include_directories(
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
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
