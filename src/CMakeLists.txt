project(aioli)

set(AIOLI__SRC 
    cpp/base.cpp
)

set(AIOLI_HDRS 
    include/base.h
)

add_library(${PROJECT_NAME} ${AIOLI_SRC} ${AIOLI_HDRS})
target_include_directories (${PROJECT_NAME} INTERFACE
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    PUBLIC $<INSTALL_INTERFACE:include>
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    VERSION ${CMAKE_PROJECT_VERSION}
    PUBLIC_HEADER "${AIOLI_HDRS}"
)
