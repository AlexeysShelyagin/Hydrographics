ADD_LIBRARY(Graphics_lib
        ${CMAKE_CURRENT_LIST_DIR}/graphics.cpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics.h
        ${CMAKE_CURRENT_LIST_DIR}/UI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/UI.h
        )

set(SFML_STATIC_LIBRARIES TRUE)
#set(SFML_DIR C:/Libraries/SFML/lib/cmake/SFML)
set(SFML_DIR ${CMAKE_CURRENT_LIST_DIR}/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS graphics window system)

#include_directories(C:/Libraries/SFML/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/SFML/include)
target_link_libraries(Graphics_lib sfml-system sfml-window sfml-graphics)