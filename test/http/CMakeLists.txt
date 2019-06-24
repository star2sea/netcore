file(GLOB SRC_FILE *.h *.hpp *.cc *.cpp)
source_group(http FILES ${SRC_FILE})

add_executable(httpserver_test ${SRC_FILE})
IF(UNIX AND NOT APPLE)
target_link_libraries(httpserver_test netcore pthread)
ELSE()
target_link_libraries(httpserver_test netcore)
ENDIF()
