include(${cmake_include}/listSubDirs.cmake)

file(GLOB_RECURSE source_group ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE include_group ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

listSubDirs(subDirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${subDirs})
    include_directories(${subdir})
endforeach(subdir)

add_library(glad ${source_group} ${include_group})
