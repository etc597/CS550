file(GLOB_RECURSE source_group ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE include_group ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${dep_root}/glad/include)

add_library(imgui ${source_group} ${include_group})
target_link_libraries(imgui glfw)
target_link_libraries(imgui glad)