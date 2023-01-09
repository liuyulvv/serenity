/**
 * @file window.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#include "window.h"

namespace serenity {

Window::Window(const std::string& title, int width, int height, const std::shared_ptr<spdlog::logger>& logger) : width_(width), height_(height), title_(title), logger_(logger) {
    glfwInit();
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

Window::~Window() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

bool Window::ShouleClose() const {
    return glfwWindowShouldClose(window_);
}

}  // namespace serenity