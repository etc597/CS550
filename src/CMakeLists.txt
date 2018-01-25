include(${cmake_include}/listSubDirs.cmake)
include(${cmake_include}/sourceGroup.cmake)

add_definitions(-DWINDOW_NAME="${PROJECT_NAME}")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB_RECURSE source_group ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE include_group ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

#listSubDirs(subDirs ${source_root})
#foreach(subdir ${subDirs})
#    include_directories(${subdir})
#endforeach(subdir)


include_directories(${source_root})

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${dep_root}/glad/include)
include_directories(${dep_root}/imgui)
include_directories(${dep_root}/glm-0.9.8.0)
include_directories(${dep_root}/stb)

add_executable(${PROJECT_NAME} ${source_group} ${include_group})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} glad)

Install(TARGETS ${PROJECT_NAME} DESTINATION ${bin_root})

sourceGroup(source_root source_group)
sourceGroup(source_root include_group)
