cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(ModelViewer)

find_package(GLFW3 REQUIRED)

include_directories(include)

add_subdirectory(glad)
add_subdirectory(lodepng)

add_library(bounding_box src/bounding_box.cpp)
add_library(constants src/constants.cpp)
add_library(io src/io.cpp)
add_library(matrix4 src/matrix4.cpp)
add_library(model_3d src/model_3d/mesh_model.cpp)
add_library(opengl_wrapper 
    src/opengl_wrapper/message_callback.cpp 
    src/opengl_wrapper/framebuffer.cpp 
    src/opengl_wrapper/texture.cpp 
    src/opengl_wrapper/vertex_array_object.cpp 
    src/opengl_wrapper/shader_program.cpp)
add_library(events 
    src/events/event_dispatcher.cpp 
    src/events/event_listener.cpp 
    src/events/mouse_events.cpp 
    src/events/keyboard_events.cpp)
add_library(obj_loader src/obj_loader.cpp)
add_library(str_tools src/str_tools.cpp)
add_executable(viewmodel src/main.cpp)

target_link_libraries(opengl_wrapper io glad)
target_link_libraries(events glad glfw matrix4 constants)
target_link_libraries(obj_loader io str_tools lodepng)
target_link_libraries(model_3d opengl_wrapper bounding_box events)
target_link_libraries(viewmodel events constants model_3d obj_loader)