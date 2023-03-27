############################################################

cmake_minimum_required(VERSION 3.5)

set(EXEC_NAME quickchat-client)

set(${EXEC_NAME}_DIR_CLIENT_SESSION client_session)

set(${EXEC_NAME}_INCLUDE_DIRS 
    ${${EXEC_NAME}_DIR_CLIENT_SESSION}
    )

set(${EXEC_NAME}_SRC
    ${${EXEC_NAME}_DIR_CLIENT_SESSION}/client_session.cpp
    ${${EXEC_NAME}_DIR_CLIENT_SESSION}/startup_client.cpp
    )

add_executable(${EXEC_NAME} ${${EXEC_NAME}_SRC})

set_target_properties(${EXEC_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${EXEC_NAME} PRIVATE
    -Wall 
    -Wextra 
    -Wpedantic 
    -std=c++20
    )

target_include_directories(${EXEC_NAME} PRIVATE 
    ${${EXEC_NAME}_INCLUDE_DIRS} 
    )

target_link_libraries(${EXEC_NAME} PRIVATE 
    quickchat-shared    
    quickchat-network
    )

############################################################