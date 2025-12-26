set(AIOLI_SRC 
    cpp/base.cpp
)

set(AIOLI_HDRS 
    include/base.h
    include/base_net.h
)

add_library(${CMAKE_PROJECT_NAME} ${AIOLI_SRC} ${AIOLI_HDRS})
target_include_directories (${CMAKE_PROJECT_NAME} INTERFACE
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
    PUBLIC $<INSTALL_INTERFACE:include>
)

set_target_properties(
    ${CMAKE_PROJECT_NAME} PROPERTIES 
    LINKER_LANGUAGE CXX
    VERSION ${CMAKE_PROJECT_VERSION}
    PUBLIC_HEADER "${AIOLI_HDRS}"
)

