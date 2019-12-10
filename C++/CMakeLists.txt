project(calgorithms)

cmake_minimum_required(VERSION 2.8)
 
SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS(-std=c++11 -g -Wall )

INCLUDE_DIRECTORIES( 
					/usr/local/include
					
					)

LINK_DIRECTORIES(
				/usr/local/lib 
				/usr/local/lib64
				)
				
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/linux/lib )
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/linux)

add_subdirectory(hyperloglog)


