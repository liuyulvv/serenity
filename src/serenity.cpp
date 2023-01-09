/**
 * @file serenity.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#include "serenity.h"

#include <fstream>

#include "spdlog/sinks/basic_file_sink.h"

namespace serenity {

Serenity::Serenity() {
    config_ = nlohmann::json::parse(std::ifstream("serenity.json"));
    logger_ = spdlog::basic_logger_mt(config_["log_name"].get<std::string>(), config_["log_path"].get<std::string>() + "log");
    logger_->set_level(spdlog::level::trace);
    window_ = std::make_unique<Window>("serenity", 800, 600, logger_);
    instance_ = std::make_unique<Instance>(logger_);
}

void Serenity::Loop() {
    while (!window_->ShouleClose()) {
        glfwPollEvents();
    }
}

}  // namespace serenity