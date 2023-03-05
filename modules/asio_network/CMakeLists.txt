############################################################

set(LIBRARY_NAME network)

set(${LIBRARY_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Inc)
set(${LIBRARY_NAME}_SOURCE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Src)

set(${LIBRARY_NAME}_SRC 
    ${${LIBRARY_NAME}_SOURCE_DIRS}/socket_data.cpp
    ${${LIBRARY_NAME}_SOURCE_DIRS}/send_receive.cpp
    ${${LIBRARY_NAME}_SOURCE_DIRS}/listen_connect.cpp)

set (BOOST_COMPONENTS thread system)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_library(${LIBRARY_NAME}_lib STATIC ${${LIBRARY_NAME}_SRC} ${${LIBRARY_NAME}_INCLUDE_DIRS}/network)

target_include_directories (${LIBRARY_NAME}_lib PUBLIC ${${LIBRARY_NAME}_INCLUDE_DIRS})
target_include_directories (${LIBRARY_NAME}_lib PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${LIBRARY_NAME}_lib PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${LIBRARY_NAME}_lib PUBLIC wsock32.a ws2_32.a)

############################################################