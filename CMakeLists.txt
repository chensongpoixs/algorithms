project(calgorithms)

cmake_minimum_required(VERSION 2.8)
 
SET(CMAKE_BUILD_TYPE Release)
ADD_DEFINITIONS(-g -Wall )


				

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_subdirectory(C++/sort)
#add_subdirectory(hyperloglog)


