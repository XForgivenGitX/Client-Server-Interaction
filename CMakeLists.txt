cmake_minimum_required(VERSION 3.0)
project(ClientServerInteraction)
#####
set (BOOST_ROOT "D:/SOFT/boost_1_81_0/boost/")
set (CMAKE_CXX_STANDARD 20) 
#####
if (WIN32)
  add_definitions(-D _WIN32_WINNT=0x0601)
endif()
#####
find_package (Boost 1.81.0) 
include_directories(${Boost_INCLUDE_DIR})
include_directories("./common")
#####
aux_source_directory("./server" MAIN)
#####
add_executable (${PROJECT_NAME} ${MAIN})
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
