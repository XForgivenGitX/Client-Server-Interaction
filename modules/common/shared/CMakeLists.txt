############################################################
cmake_minimum_required(VERSION 3.5)

set(LIBRARY_NAME shared)

set(${LIBRARY_NAME}_DIR_LOG ${CMAKE_CURRENT_SOURCE_DIR}/log)
set(${LIBRARY_NAME}_DIR_PROTOCOL ${CMAKE_CURRENT_SOURCE_DIR}/protocol)

set(${LIBRARY_NAME}_INCLUDE_DIRS 
    ${${LIBRARY_NAME}_DIR_LOG}
    ${${LIBRARY_NAME}_DIR_PROTOCOL}
    )

set(${LIBRARY_NAME}_SRC
    ${${LIBRARY_NAME}_DIR_LOG}/logger.cpp
    ${${LIBRARY_NAME}_DIR_PROTOCOL}/protocol.cpp
    )

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS log_setup log)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_library(${LIBRARY_NAME} STATIC ${${LIBRARY_NAME}_SRC})

set_target_properties(${LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${LIBRARY_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -std=c++17
    )

target_include_directories (${LIBRARY_NAME} 
    PUBLIC ${${LIBRARY_NAME}_INCLUDE_DIRS} 
    PRIVATE ${Boost_INCLUDE_DIRS}
    )

target_link_libraries(${LIBRARY_NAME} PRIVATE ${Boost_LIBRARIES})

############################################################