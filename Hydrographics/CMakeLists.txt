cmake_minimum_required(VERSION 3.20)
project(Hydrographics)

set(CMAKE_CXX_STANDARD 17)

add_executable(Hydrographics main.cpp model_loader.h model_loader.cpp mesh.h vectors.h vectors.cpp mesh.cpp renderer.cpp renderer.h slicer.h slicer.cpp)

include(./include/graphics/CMakeLists.txt)
target_link_libraries(Hydrographics Graphics_lib)

include_directories(C:/Libraries/tinyobjloader)