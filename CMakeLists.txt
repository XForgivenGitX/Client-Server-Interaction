cmake_minimum_required(VERSION 3.0)
#####
set (PROJECT ClientServerInteraction)

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
set (Boost_USE_MULTITHREADED ON)
set (BOOST_COMPONENTS thread system)
set (Boost_USE_STATIC_RUNTIME OFF)
#####
project(${PROJECT})
#####
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${INCLUDES})
#####
#add_executable (server ${SERVER_SOURCES} ${MY_LIB})
add_executable (client ${CLIENT_SOURCES} ${MY_LIB})
#####
#target_link_libraries (server ${Boost_LIBRARIES})
target_link_libraries (client ${Boost_LIBRARIES})
target_link_libraries (client wsock32.a ws2_32.a)
# https://translated.turbopages.org/proxy_u/en-ru.ru.1536f157-64031203-0a61f793-74722d776562/https/code.visualstudio.com/docs/cpp/config-mingw