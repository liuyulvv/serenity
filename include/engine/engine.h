//
// Created by liuyulvv on 2022/10/30.
//

#ifndef SERENITY_ENGINE_H
#define SERENITY_ENGINE_H

#include <memory>

#include "json.hpp"
#include "spdlog.h"
#include "window.h"

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
    nlohmann::json config_;
    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<Window> window_;
};

}  // namespace serenity

#endif  // SERENITY_ENGINE_H
