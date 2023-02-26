cmake_minimum_required(VERSION 3.0)
project(ClientServerInteraction)
add_subdirectory(server)

#set (BOOST_ROOT "D:/SOFT/boost_1_81_0/boost/")
set (CMAKE_CXX_STANDARD 20) 

#find_package (Boost 1.81.0) 
#include_directories (SYSTEM ${Boost_INCLUDE_DIR})
#target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
