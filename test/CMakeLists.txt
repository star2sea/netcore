add_executable(timestamp_test timestamp_test.cpp)
add_executable(threadpool_test threadpool_test.cpp)
target_link_libraries(threadpool_test netcore)

add_executable(server_test server_test.cpp)
target_link_libraries(server_test netcore)

add_executable(client_test client_test.cpp)
target_link_libraries(client_test netcore)

add_subdirectory(http)