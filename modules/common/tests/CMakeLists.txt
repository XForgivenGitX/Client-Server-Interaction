############################################################

cmake_minimum_required(VERSION 3.5)

set(TESTS_NAME test-protocol)

set(${TESTS_NAME}_SRC
    protocol.cpp
    )

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS unit_test_framework)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

add_executable(${TESTS_NAME} ${${TESTS_NAME}_SRC})

set_target_properties(${TESTS_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

target_include_directories (${TESTS_NAME} PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(${TESTS_NAME} PRIVATE 
    ${Boost_LIBRARIES}
    quickchat-shared
    )

add_test(NAME ${TESTS_NAME} COMMAND ${TESTS_NAME})

############################################################