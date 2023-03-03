cmake_minimum_required(VERSION 3.0)
#####
set (PROJECT ClientServerInteraction)

set (BOOST_LIBRARYDIR "/SOFT/boost_1_81_0/boost/lib")
set (Boost_INCLUDE_DIRS "/SOFT/boost_1_81_0/boost/include/boost-1_81")

set (CMAKE_CXX_STANDARD 20) 
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set (INCLUDES
  "network_lib/network_io/Inc/"
  "network_lib/tasks/Inc/"
  "server/Inc"
  "client/Inc"
  "./"
)
set(MY_LIB
"network_lib/network_io/src/SocketData.cpp"
"network_lib/network_io/src/SendReceive.cpp"
"network_lib/tasks/Src/taskProcessor.cpp"
"network_lib/tasks/Src/taskWrapped.cpp"
)

set (SERVER_SOURCES 
  "server/Src/main.cpp" 
  "server/Src/handlers.cpp"
  "server/Src/listen.cpp"
  ) 
set (CLIENT_SOURCES 
  "client/Src/main.cpp" 
  "client/Src/connection.cpp"
  "client/Src/session.cpp"
  ) 

#####
set (BOOSTROOT "/SOFT/boost_1_81_0/boost")
if(BUILD_SHARED_LIBS)
    add_definitions(-DBOOST_ALL_DYN_LINK)  
else()  
    set (Boost_USE_STATIC_LIBS ON)
endif()
set (Boost_USE_MULTITHREADED ON)
set (BOOST_COMPONENTS thread system)
set(Boost_USE_STATIC_RUNTIME OFF)
#####
project(${PROJECT})
#####
if (WIN32)
  add_definitions(-D _WIN32_WINNT=0x0601)
endif()
#####
#find_library(BOOST_ROOT Boost)
find_package(Boost 1.81.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${INCLUDES})
#####
#add_executable (server ${SERVER_SOURCES} ${MY_LIB})
add_executable (client ${CLIENT_SOURCES} ${MY_LIB})
#####
#target_link_libraries (server ${Boost_LIBRARIES})
target_link_libraries (client ${Boost_LIBRARIES})
target_compile_features(
client
PRIVATE
cxx_auto_type
cxx_range_for)