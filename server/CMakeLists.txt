#########################################################

set(SUBPROJECT_NAME server)

set(${SUBPROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Inc)
set(${SUBPROJECT_NAME}_SOURCE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Src)

set(${SUBPROJECT_NAME}_SRC
    ${${SUBPROJECT_NAME}_SOURCE_DIRS}/server_handlers.cpp
    ${${SUBPROJECT_NAME}_SOURCE_DIRS}/server_process.cpp)

add_executable(${SUBPROJECT_NAME}  ${${SUBPROJECT_NAME}_SRC})

target_include_directories(${SUBPROJECT_NAME} PRIVATE ${${SUBPROJECT_NAME}_INCLUDE_DIRS})
target_link_libraries(${SUBPROJECT_NAME} PRIVATE network_lib)

#########################################################