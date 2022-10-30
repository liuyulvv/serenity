#include <fstream>

#include "glad/glad.h"
#include "glfw3.h"
#include "json.hpp"
#include "spdlog.h"
#include "spdlog/fmt/chrono.h"
#include "spdlog/sinks/basic_file_sink.h"

using json = nlohmann::json;

int main() {
    json config = json::parse(std::ifstream("config.json"));

    auto logger = spdlog::basic_logger_mt(config["log_name"].get<std::string>(), config["log_path"].get<std::string>() + "log");
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config["opengl_major_version"].get<int>());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config["opengl_minor_version"].get<int>());
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(config["window_width"].get<int>(), config["window_height"].get<int>(), config["window_title"].get<std::string>().c_str(), nullptr, nullptr);
    if (window == nullptr) {
        logger->error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logger->error("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, config["window_width"].get<int>(), config["window_height"].get<int>());

    auto window_resize_callback = [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    };
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    auto process_input = [&]() {
#ifdef __APPLE__
        // command + w: close window
        if ((glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
            logger->info("Exit");
        }
#endif
    };

    auto clear_color_red = config["clear_color_red"].get<float>();
    auto clear_color_green = config["clear_color_green"].get<float>();
    auto clear_color_blue = config["clear_color_blue"].get<float>();
    auto clear_color_alpha = config["clear_color_alpha"].get<float>();

    while (!glfwWindowShouldClose(window)) {
        process_input();
        glClearColor(clear_color_red, clear_color_green, clear_color_blue, clear_color_alpha);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}