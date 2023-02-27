cmake_minimum_required(VERSION 3.0)
#####
set (PROJECT ClientServerInteraction)
set (CMAKE_CXX_STANDARD 20) 
set (INCLUDES
  "./common/Inc"
  "./server/Inc"
  "./"
)
set (SERVER_SOURCES 
  "server/Src/main.cpp"  
  "server/Src/ConnectionData.cpp"
  "common/Src/TaskProcessor.cpp"
  "common/Src/TaskWrapped.cpp"
) 
#####
set (BOOST_ROOT "D:/SOFT/boost_1_81_0/boost/")
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
set (BOOST_COMPONENTS thread system)
#####
project(${PROJECT})
#####
if (WIN32)
  add_definitions(-D _WIN32_WINNT=0x0601)
endif()
#####
find_package(Boost 1.81.0 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${INCLUDES})
#####
add_executable (${PROJECT_NAME} ${SERVER_SOURCES})
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})