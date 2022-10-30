//
// Created by liuyulvv on 2022/10/30.
//

#ifndef SERENITY_ENGINE_H
#define SERENITY_ENGINE_H

#include <memory>

#include "glad/glad.h"
#include "glfw3.h"
#include "json.hpp"
#include "spdlog.h"

namespace serenity {

class Engine {
public:
    Engine();
    virtual ~Engine();

    Engine(const Engine&& engine) = delete;
    Engine(const Engine& engine) = delete;
    void operator=(const Engine& engine) = delete;
    void operator=(const Engine&& engine) = delete;

public:
    void Loop();

private:
    void ProcessInput();

private:
    nlohmann::json config_;
    std::shared_ptr<spdlog::logger> logger_;
    GLFWwindow* window_;
    float clear_color_red_;
    float clear_color_green_;
    float clear_color_blue_;
    float clear_color_alpha_;
};

}  // namespace serenity

#endif  // SERENITY_ENGINE_H
