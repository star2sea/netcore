file(GLOB SRC_FILE *.h *.hpp *.cc *.cpp)
source_group(http FILES ${SRC_FILE})
add_executable(httpserver_test ${SRC_FILE})
target_link_libraries(httpserver_test netcore http_parser pthread)
