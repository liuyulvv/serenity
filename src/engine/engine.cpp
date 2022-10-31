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
    window_ = std::make_shared<Window>(config_, logger_);
}

Engine::~Engine() {
    logger_->flush();
}

void Engine::Loop() {
    logger_->info("Start");
    while (!window_->ShouldClose()) {
        window_->ProcessInput();
        window_->Clear();
        window_->SwapBuffer();
        window_->PollEvents();
    }
    logger_->info("Exit");
}

}  // namespace serenity