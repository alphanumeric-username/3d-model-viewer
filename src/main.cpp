#include "constants.hpp"
#include "bounding_box.hpp"
#include "obj_loader.hpp"
#include "matrix4.hpp"
#include "model_3d/mesh_model.hpp"
#include "events/event_dispatcher.hpp"
#include "events/mouse_events.hpp"
#include "events/keyboard_events.hpp"
#include "opengl_wrapper/shader_program.hpp"
#include "opengl_wrapper/vertex_array_object.hpp"
#include "opengl_wrapper/message_callback.hpp"
#include "opengl_wrapper/framebuffer.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <filesystem>
#include <tuple>
#include <memory>
#include <sstream>

namespace fs = std::filesystem;

void on_error(int error, const char* description);
void mouse_button_callback(GLFWwindow* win, int button, int action, int mods);
void mouse_cursor_callback(GLFWwindow* win, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* win, double xoffset, double yoffset);

GLFWwindow* createWindow(int width, int height, std::string title);
void setup_opengl();
app::MeshModel load_model(fs::path path_to_model);

app::EventDispatcher event_dispatcher{};

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "Usage: viewmodel path/to/model.obj\n";
        return 1;
    }

    fs::path model_file_name { argv[1] };

    if(!glfwInit()) {
        std::cerr << "[GLFW]: Could not initialize\n";
        return 1;
    }

    std::stringstream ss_win_name{};
    ss_win_name << "Model: " << model_file_name.stem();

    auto win { createWindow(VP_WIDTH, VP_HEIGHT, ss_win_name.str()) };

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "[GLAD]: Could not load Open GL functions\n";
        glfwTerminate();
        return 1;
    }

    setup_opengl();

    auto mesh_model { load_model(model_file_name) };

    auto model_bb { mesh_model.bounding_box() };

    app::ShaderProgram model_shader {
        "../data/shaders/default/vertex_shader.glsl",
        "../data/shaders/default/fragment_shader.glsl"
    };

    mesh_model.set_shader_program(model_shader);
    mesh_model.send_to_gpu();

    auto model_mouse_listener {
        std::make_shared<app::MouseListener>()
    };
    
    event_dispatcher.subscribe(model_mouse_listener, app::EventType::MOUSE_CLICK);
    event_dispatcher.subscribe(model_mouse_listener, app::EventType::MOUSE_MOVE);
    event_dispatcher.subscribe(model_mouse_listener, app::EventType::MOUSE_SCROLL);

    mesh_model.set_mouse_listener(model_mouse_listener);

    while(!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        model_shader.use();
        mesh_model.draw();

        glfwSwapBuffers(win);
    }

    glfwTerminate();

    return 0;
}

void on_error(int error, const char* description) {
    std::cerr << "[GLFW]: Code: " << error << " Description: " << description << '\n';
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
    event_dispatcher.on_mouse_click(win, button, action, mods);
}

void mouse_cursor_callback(GLFWwindow* win, double xpos, double ypos) {
    event_dispatcher.on_mouse_move(win, xpos, ypos);
}

void mouse_scroll_callback(GLFWwindow* win, double xoffset, double yoffset) {
    event_dispatcher.on_mouse_scroll(win, xoffset, yoffset);
}

GLFWwindow* createWindow(int width, int height, std::string title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* win { 
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr)
    };

    glfwMakeContextCurrent(win);

    glfwSetMouseButtonCallback(win, mouse_button_callback);
    glfwSetCursorPosCallback(win, mouse_cursor_callback);
    glfwSetScrollCallback(win, mouse_scroll_callback);
    glfwSetKeyCallback(win, app::keyboard::on_key);

    return win;
}

void setup_opengl() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDebugMessageCallback(app::opengl_message_callback, nullptr);
    glViewport(0, 0, VP_WIDTH, VP_HEIGHT);
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

app::MeshModel load_model(fs::path path_to_model) {
    auto meshes { app::obj_loader::load_obj(path_to_model) };
    app::MeshModel mesh_model{};
    mesh_model.set_meshes(meshes);

    return mesh_model;
}