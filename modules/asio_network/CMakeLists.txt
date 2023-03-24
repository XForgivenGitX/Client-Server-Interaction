############################################################

set(LIBRARY_NAME network)

set(${LIBRARY_NAME}_INC 
    ${CMAKE_CURRENT_LIST_DIR}/socket_data/utility.hpp    
    ${CMAKE_CURRENT_LIST_DIR}/socket_data/socket_data.hpp
    ${CMAKE_CURRENT_LIST_DIR}/socket_io/send_receive.hpp
    ${CMAKE_CURRENT_LIST_DIR}/socket_io/listen_connect.hpp
    )


set(${LIBRARY_NAME}_SRC 
    ${CMAKE_CURRENT_LIST_DIR}/socket_data/socket_data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/socket_io/send_receive.cpp
    ${CMAKE_CURRENT_LIST_DIR}/socket_io/listen_connect.cpp
    )

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_library(${LIBRARY_NAME}_lib STATIC)
target_sources(${LIBRARY_NAME}_lib ${${LIBRARY_NAME}_SRC})

target_include_directories (${LIBRARY_NAME}_lib PUBLIC ${${LIBRARY_NAME}_INC})
target_include_directories (${LIBRARY_NAME}_lib PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${LIBRARY_NAME}_lib PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${LIBRARY_NAME}_lib PUBLIC wsock32.a ws2_32.a)

############################################################