cmake_minimum_required(VERSION 3.0)
set (BOOST_ROOT "D:/SOFT/boost_1_81_0/boost/") # Путь к библиотеке Boost
 
set (CMAKE_CXX_STANDARD 20) 
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)
  
find_package (Boost REQUIRED COMPONENTS program_options REQUIRED) 
  
if (Boost_FOUND)
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
    add_executable (${PROJECT_NAME} "main.cpp")
    target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
endif ()