add_executable(timestamp_test timestamp_test.cpp)

add_executable(logger_test logger_test.cpp)
target_link_libraries(logger_test netcore)

add_executable(threadpool_test threadpool_test.cpp)
IF(UNIX)
target_link_libraries(threadpool_test netcore pthread)
ELSE()
target_link_libraries(threadpool_test netcore)
ENDIF()


add_executable(server_test server_test.cpp)
IF(UNIX)
target_link_libraries(server_test netcore pthread)
ELSE()
target_link_libraries(server_test netcore)
ENDIF()


add_executable(client_test client_test.cpp)
IF(UNIX)
target_link_libraries(client_test netcore pthread)
ELSE()
target_link_libraries(client_test netcore)
ENDIF()

add_subdirectory(http)
add_subdirectory(protobuf)