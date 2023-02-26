cmake_minimum_required(VERSION 3.0)
project(ClientServerInteraction)
#####
set (BOOST_ROOT "D:/SOFT/boost_1_81_0/boost/")
set (CMAKE_CXX_STANDARD 20) 
#####
set (INCLUDE 
  "./common/Inc/"
  
)
set (SOURCES 
  "server/main.cpp"  
  "common/Src/TaskProcessor.cpp"

) 
#####
if (WIN32)
  add_definitions(-D _WIN32_WINNT=0x0601 -O0)
endif()
#####
find_package (Boost 1.81.0) 
include_directories(${Boost_INCLUDE_DIR})
include_directories(${INCLUDE})
#####
add_executable (${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
