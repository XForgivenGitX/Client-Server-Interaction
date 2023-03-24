############################################################

cmake_minimum_required(VERSION 3.5)

set(LIBRARY_NAME network)

set(${LIBRARY_NAME}_DIR_SOCKET_DATA socket_data)
set(${LIBRARY_NAME}_DIR_SOCKET_IO socket_io)

set(${LIBRARY_NAME}_INCLUDE_DIRS 
    ${${LIBRARY_NAME}_DIR_SOCKET_DATA}/Inc
    ${${LIBRARY_NAME}_DIR_SOCKET_IO}/Inc
    )

set(${LIBRARY_NAME}_SOCKET_DATA_SOURCE_DIR ${${LIBRARY_NAME}_DIR_SOCKET_DATA}/Src)
set(${LIBRARY_NAME}_SOCKET_IO_SOURCE_DIR ${${LIBRARY_NAME}_DIR_SOCKET_IO}/Src)

set(${LIBRARY_NAME}_SRC
    ${${LIBRARY_NAME}_SOCKET_DATA_SOURCE_DIR}/socket_data.cpp
    ${${LIBRARY_NAME}_SOCKET_IO_SOURCE_DIR}/send_receive.cpp
    ${${LIBRARY_NAME}_SOCKET_IO_SOURCE_DIR}/listen_connect.cpp
    )

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_library(${LIBRARY_NAME} STATIC ${${LIBRARY_NAME}_SRC})

set_target_properties(${LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${LIBRARY_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -std=c++17
    )

target_include_directories(${LIBRARY_NAME} PRIVATE 
    ${${LIBRARY_NAME}_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS}
    )

    target_link_libraries(${LIBRARY_NAME} PRIVATE 
    ${Boost_LIBRARIES} 
    wsock32.a 
    ws2_32.a
    )

############################################################