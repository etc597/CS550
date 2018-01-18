cmake_minimum_required(VERSION 3.6.0)
project(CS550_Physics_Simulation)

set(root ${CMAKE_CURRENT_SOURCE_DIR})
set(bin_root ${root}/bin)
set(source_root ${root}/src)
set(dep_root ${root}/dep)
set(cmake_include ${root}/cmake)
set(asset_path ${root}/assets)

set(CMAKE_CXX_STANDARD 11)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(dep)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
add_subdirectory(src)