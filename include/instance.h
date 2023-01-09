/**
 * @file instance.h
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#if !defined(SERENITY_INSTANCE_H_)
#define SERENITY_INSTANCE_H_

#include <memory>
#include <vector>

#include "glfw3.h"
#include "spdlog.h"
#include "vulkan/vulkan.h"

namespace serenity {

class Instance {
public:
    explicit Instance(const std::shared_ptr<spdlog::logger>& logger);
    ~Instance();

    Instance() = delete;
    Instance(const Instance& instance) = delete;
    Instance& operator=(const Instance& instance) = delete;
    Instance(Instance&& instance) = delete;
    Instance& operator=(Instance&& instance) = delete;

private:
    void CreateInstance();
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions() const;
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
    void SetupDebugMessenger();
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator);
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);

private:
    VkInstance instance_;
    std::shared_ptr<spdlog::logger> logger_;
    const std::vector<const char*> VALIDATION_LAYERS_{"VK_LAYER_KHRONOS_validation"};
#if defined(NODEBUG)
    const bool ENABLE_VALIDATION_LAYERS_ = false;
#else
    const bool ENABLE_VALIDATION_LAYERS_ = true;
#endif  // NODEBUG
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
};

}  // namespace serenity

#endif  // SERENITY_INSTANCE_H_