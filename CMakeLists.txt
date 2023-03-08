############################################

cmake_minimum_required(VERSION 3.0)
project(LocalChat)

set (CMAKE_CXX_STANDARD 20) 
set (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(modules/asio_network)
add_subdirectory(server)
# add_subdirectory(client)

############################################

# https://habr.com/ru/post/197392/
# https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c