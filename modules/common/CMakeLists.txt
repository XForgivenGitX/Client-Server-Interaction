############################################################

set(LIBRARY_NAME common)

set(${LIBRARY_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Inc)
set(${LIBRARY_NAME}_SOURCE_DIRS ${CMAKE_CURRENT_LIST_DIR}/Src)

set(${LIBRARY_NAME}_SRC 
    ${${LIBRARY_NAME}_SOURCE_DIRS}/protocol.cpp)

add_library(${LIBRARY_NAME}_lib STATIC ${${LIBRARY_NAME}_SRC} ${${LIBRARY_NAME}_INCLUDE_DIRS})

target_include_directories (${LIBRARY_NAME}_lib PUBLIC ${${LIBRARY_NAME}_INCLUDE_DIRS})

############################################################