############################################################

cmake_minimum_required(VERSION 3.5)

set(EXEC_NAME ${PROJECT_NAME}-server)

set(${EXEC_NAME}_DIR_ACCEPTOR acceptor)
set(${EXEC_NAME}_DIR_DATABASE database)
set(${EXEC_NAME}_DIR_CHANNELS channel)
set(${EXEC_NAME}_DIR_CONNECTIONS connections)

set(${EXEC_NAME}_INCLUDE_DIRS 
    ${${EXEC_NAME}_DIR_DATABASE}
    ${${EXEC_NAME}_DIR_CHANNELS}
    ${${EXEC_NAME}_DIR_CONNECTIONS}
    ${${EXEC_NAME}_DIR_ACCEPTOR}
    )

set(${EXEC_NAME}_SRC
    ${${EXEC_NAME}_DIR_DATABASE}/database.cpp
    ${${EXEC_NAME}_DIR_CHANNELS}/chat_member.cpp
    ${${EXEC_NAME}_DIR_CHANNELS}/channel.cpp
    ${${EXEC_NAME}_DIR_ACCEPTOR}/acceptor.cpp
    ${${EXEC_NAME}_DIR_ACCEPTOR}/startup_server.cpp
    ${${EXEC_NAME}_DIR_CONNECTIONS}/connections_manager.cpp
    )

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_executable(${EXEC_NAME} ${${EXEC_NAME}_SRC})

set_target_properties(${EXEC_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${EXEC_NAME} PRIVATE
    -Wall 
    -Wextra
    -Wpedantic 
    -std=c++17 
    -DSERVER_ENABLE_HANDLER_TRACKING
    -DDB_LOG_SEVERITY=info
    -DSERVER_LOG_SEVERITY=info
    )

target_include_directories(${EXEC_NAME} PRIVATE 
    ${${EXEC_NAME}_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS}
    )

target_link_libraries(${EXEC_NAME} PRIVATE 
    ${Boost_LIBRARIES} 
    ${PROJECT_NAME}-shared
    ${PROJECT_NAME}-network
    )

############################################################