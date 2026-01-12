set(AIOLI_SRC 
    cpp/base.cpp
    cpp/base_net.cpp
    cpp/event_pollers/base.cpp
    cpp/event_pollers/select.cpp
)

set(AIOLI_HDRS 
    include/aioli/base.h
    include/aioli/base_net.h
    include/aioli/timer.h
    include/aioli/event_loop.h
    include/aioli/event_pollers/types.h
    include/aioli/event_pollers/base.h
    include/aioli/event_pollers/select.h
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

