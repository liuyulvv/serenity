/**
 * @file instance.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @date 2023-01-06
 */

#include "instance.h"

#include <span>
#include <string>

namespace serenity {

Instance::Instance(const std::shared_ptr<spdlog::logger>& logger) : logger_(logger) {
    CreateInstance();
    SetupDebugMessenger();
}

Instance::~Instance() {
    if (ENABLE_VALIDATION_LAYERS_) {
        DestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
    }
    vkDestroyInstance(instance_, nullptr);
}

void Instance::CreateInstance() {
    if (ENABLE_VALIDATION_LAYERS_ && !CheckValidationLayerSupport()) {
        throw std::runtime_error("Validation layers requested, but not available.");
    }
    // application info
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.pApplicationName = "serenity";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "serenity";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // extension
    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    create_info.pApplicationInfo = &app_info;

    auto extensions = GetRequiredExtensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (ENABLE_VALIDATION_LAYERS_) {
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS_.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS_.data();
        PopulateDebugMessengerCreateInfo(debug_create_info);
        create_info.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debug_create_info);
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance.");
    }
}

bool Instance::CheckValidationLayerSupport() {
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    for (const auto& layer_name : VALIDATION_LAYERS_) {
        bool layer_found = false;
        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, static_cast<const char*>(layer_properties.layerName)) == 0) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            return false;
        }
    }
    return true;
}

std::vector<const char*> Instance::GetRequiredExtensions() const {
    uint32_t glfw_extension_count = 0;
    const auto** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::span<const char*> glfw_required_extensions(glfw_extensions, glfw_extension_count);
    std::vector<const char*> extensions(glfw_required_extensions.begin(), glfw_required_extensions.end());
    if (ENABLE_VALIDATION_LAYERS_) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    return extensions;
}

VkResult Instance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger) {
    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        return func(instance, create_info, allocator, debug_messenger);
    }
    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Instance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = DebugCallback;
    create_info.pUserData = &logger_;
}

void Instance::SetupDebugMessenger() {
    if (!ENABLE_VALIDATION_LAYERS_) {
        return;
    }
    VkDebugUtilsMessengerCreateInfoEXT create_info;
    PopulateDebugMessengerCreateInfo(create_info);
    if (CreateDebugUtilsMessengerEXT(instance_, &create_info, nullptr, &debug_messenger_) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger.");
    }
}

void Instance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        func(instance, debug_messenger, allocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Instance::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
    auto* logger_ = static_cast<std::shared_ptr<spdlog::logger>*>(user_data);
    std::string type{};
    std::string message(callback_data->pMessage);
    if (!message.empty()) {
        switch (message_type) {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: {
                type = "GENERAL";
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: {
                type = "VALIDATION";
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: {
                type = "PERFORMANCE";
                break;
            }
        }
        switch (message_severity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
                (*logger_)->warn("Validation layer({}): {}", type, message);
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
                (*logger_)->error("Validation layer({}): {}", type, message);
                break;
            }
            default: {
                (*logger_)->info("Validation layer({}): {}", type, message);
                break;
            }
        }
    }
    return VK_FALSE;
}

}  // namespace serenity