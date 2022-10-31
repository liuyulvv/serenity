//
// Created by liuyulvv on 2022/10/31.
//

#include "window.h"

namespace serenity {

Window::Window(nlohmann::json& config, std::shared_ptr<spdlog::logger>& logger) : config_(config), logger_(logger) {
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

    glfwSetWindowUserPointer(window_, this);

    glViewport(0, 0, config_["window_width"].get<int>(), config_["window_height"].get<int>());

    auto window_resize_callback = [](GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        glViewport(0, 0, width, height);
#ifdef __APPLE__
        self->GetLogger()->info("Change window size to width: {}, height: {}", width / 2, height / 2);
#else
        self->GetLogger()->info("Change window size to width: {}, height: {}", width, height);
#endif
    };
    glfwSetFramebufferSizeCallback(window_, window_resize_callback);

    clear_color_red_ = config_["clear_color_red"].get<float>();
    clear_color_green_ = config_["clear_color_green"].get<float>();
    clear_color_blue_ = config_["clear_color_blue"].get<float>();
    clear_color_alpha_ = config_["clear_color_alpha"].get<float>();
}

Window::~Window() {
    glfwTerminate();
}

void Window::ProcessInput() {
#ifdef __APPLE__
    // command + w: close window
    if ((glfwGetKey(window_, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS) && glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, 1);
    }
#endif
}

std::shared_ptr<spdlog::logger> Window::GetLogger() const {
    return logger_;
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window_) != 0;
}

void Window::SwapBuffer() {
    glfwSwapBuffers(window_);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::Clear() const {
    glClearColor(clear_color_red_, clear_color_green_, clear_color_blue_, clear_color_alpha_);
    glClear(GL_COLOR_BUFFER_BIT);
}

}  // namespace serenity