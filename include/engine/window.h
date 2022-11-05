//
// Created by liuyulvv on 2022/10/31.
//

#ifndef SERENITY_WINDOW_H
#define SERENITY_WINDOW_H

#include <memory>

extern "C" {
#include "glad/glad.h"
#include "glfw3.h"
}

#include "json.hpp"
#include "spdlog.h"

namespace serenity {

class Window {
public:
    Window(nlohmann::json& config, std::shared_ptr<spdlog::logger>& logger);
    virtual ~Window();

    Window(const Window& window) = delete;
    Window(const Window&& window) = delete;
    void operator=(Window& window) = delete;
    void operator=(Window&& window) = delete;

public:
    void ProcessInput();
    [[nodiscard]] std::shared_ptr<spdlog::logger> GetLogger() const;
    [[nodiscard]] bool ShouldClose() const;
    void SwapBuffer();
    static void PollEvents();
    void Clear() const;

private:
    GLFWwindow* window_;
    nlohmann::json& config_;
    std::shared_ptr<spdlog::logger> logger_;

private:
    float clear_color_red_;
    float clear_color_green_;
    float clear_color_blue_;
    float clear_color_alpha_;
};

}  // namespace serenity

#endif  // SERENITY_WINDOW_H
