project(chyperloglog)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
MESSAGE(STATUS "#########################")

file(GLOB CHYPERLOGLOG
                      chyperloglog.cpp
)

file(GLOB CHYPERLOGLOG_TEST
                      chyperloglog_test.cpp
)

file(GLOB CHYPERLOGLOG_COUNT
                      chyperloglog_count.cpp
)

set(ALL_LIB  	pthread
				)

add_executable(chyperloglog ${CHYPERLOGLOG})
target_link_libraries(chyperloglog ${ALL_LIB})
add_executable(chyperloglog_test ${CHYPERLOGLOG_TEST})
target_link_libraries(chyperloglog_test ${ALL_LIB})
add_executable(chyperloglog_test ${CHYPERLOGLOG_COUNT})
target_link_libraries(chyperloglog_count ${ALL_LIB})