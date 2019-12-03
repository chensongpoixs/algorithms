project(chyperloglog)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "#########################")

file(GLOB ALL_CXX_SRCS
                       ./*.cpp  ./*.cc
)

add_executable(${PROJECT_NAME} ${ALL_CXX_SRCS})

set(ALL_LIB  	pthread
				)


target_link_libraries(${PROJECT_NAME} ${ALL_LIB})