//
// Created by liuyulvv on 2022/10/30.
//

#include "engine.h"

#include <fstream>

#include "spdlog/sinks/basic_file_sink.h"

namespace serenity {

Engine::Engine() {
    config_ = nlohmann::json::parse(std::ifstream("config.json"));
    logger_ = spdlog::basic_logger_mt(config_["log_name"].get<std::string>(), config_["log_path"].get<std::string>() + "log");
    logger_->set_level(spdlog::level::trace);
    set_default_logger(logger_);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config_["opengl_major_version"].get<int>());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config_["opengl_minor_version"].get<int>());
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window_ = glfwCreateWindow(config_["window_width"].get<int>(), config_["window_height"].get<int>(), config_["window_title"].get<std::string>().c_str(), nullptr, nullptr);
    if (window_ == nullptr) {
        logger_->error("Failed to create GLFW window");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        logger_->error("Failed to initialize GLAD");
        glfwTerminate();
        exit(1);
    }
    glViewport(0, 0, config_["window_width"].get<int>(), config_["window_height"].get<int>());

    auto window_resize_callback = [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    };
    glfwSetFramebufferSizeCallback(window_, window_resize_callback);

    clear_color_red_ = config_["clear_color_red"].get<float>();
    clear_color_green_ = config_["clear_color_green"].get<float>();
    clear_color_blue_ = config_["clear_color_blue"].get<float>();
    clear_color_alpha_ = config_["clear_color_alpha"].get<float>();
}

Engine::~Engine() {
    logger_->flush();
    glfwTerminate();
}

void Engine::Loop() {
    logger_->info("Start");
    while (!glfwWindowShouldClose(window_)) {
        ProcessInput();
        glClearColor(clear_color_red_, clear_color_green_, clear_color_blue_, clear_color_alpha_);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    logger_->info("Exit");
}

void Engine::ProcessInput() {
#ifdef __APPLE__
    // command + w: close window
    if ((glfwGetKey(window_, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS) && glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, 1);
    }
#endif
}

}  // namespace serenity