file(GLOB SRC_FILE *.h *.hpp *.cc *.cpp)
source_group(http FILES ${SRC_FILE})

add_executable(httpserver_test ${SRC_FILE})
IF(UNIX)
target_link_libraries(httpserver_test netcore http_parser pthread)
ELSE(UNIX)
target_link_libraries(httpserver_test netcore http_parser)
ENDIF(UNIX)
