cmake_minimum_required(VERSION 3.6.0)
project(physics-sim)

set(root ${CMAKE_CURRENT_SOURCE_DIR})
set(bin_root ${root}/bin)
set(source_root ${root}/src)
set(dep_root ${root}/dep)
set(cmake_include ${root}/cmake)
set(asset_path ${root}/assets)

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};Publish")
set(CMAKE_CXX_FLAGS_PUBLISH "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_PUBLISH "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_PUBLISH "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_PUBLISH "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

add_subdirectory(dep)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
add_subdirectory(src)