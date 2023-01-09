/**
 * @file window.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#if !defined(SERENITY_WINDOW_H_)
#define SERENITY_WINDOW_H_

#include <functional>
#include <memory>
#include <string>

#include "glfw3.h"
#include "spdlog.h"

namespace serenity {

class Window {
public:
    Window(const std::string& title, int width, int height, const std::shared_ptr<spdlog::logger>& logger);
    ~Window();

    Window(const Window& window) = delete;
    Window& operator=(const Window& window) = delete;
    Window(Window&& window) = delete;
    Window& operator=(Window&& window) = delete;

public:
    bool ShouleClose() const;

private:
    int width_{0};
    int height_{0};
    std::string title_{};
    GLFWwindow* window_{nullptr};
    std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace serenity

#endif  // SERENITY_WINDOW_H_