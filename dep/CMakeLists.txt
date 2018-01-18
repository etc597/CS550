add_subdirectory(glfw-3.2.1)
add_subdirectory(glad)
add_subdirectory(imgui)

set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")
