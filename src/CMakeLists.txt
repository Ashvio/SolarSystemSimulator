SET(pwd ${CMAKE_CURRENT_LIST_DIR})

SET(src "")
AUX_SOURCE_DIRECTORY(${pwd} src)
add_executable(solar ${src})
message(STATUS "solar added ${src}")

target_link_libraries(solar ${stdgl_libraries})
FIND_PACKAGE(JPEG REQUIRED)
TARGET_LINK_LIBRARIES(solar ${JPEG_LIBRARIES})

FIND_LIBRARY(libconfig REQUIRED)
TARGET_LINK_LIBRARIES(solar ${libconfig_LIBS})
