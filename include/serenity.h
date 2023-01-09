/**
 * @file serenity.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#if !defined(SERENITY_SERENITY_H_)
#define SERENITY_SERENITY_H_

#include <memory>

#include "instance.h"
#include "json.hpp"
#include "spdlog.h"
#include "window.h"

namespace serenity {

class Serenity {
public:
    Serenity();
    ~Serenity() = default;

public:
    void Loop();

private:
    nlohmann::json config_;
    std::shared_ptr<spdlog::logger> logger_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Instance> instance_;
};

}  // namespace serenity

#endif  // SERENITY_SERENITY_H_