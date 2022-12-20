/**
 * @file hello_triangle_application.cpp
 * @author liuyulvv (liuyulvv@outlook.com)
 * @version 0.1
 * @date 2022-12-20
 */

// #include <cstdlib>
#include <iostream>
#include <map>
#include <span>
#include <string>
#include <vector>

#include "glfw3.h"
#include "vulkan/vulkan.h"

class HelloTriangleApplication {
public:
    HelloTriangleApplication() = default;
    HelloTriangleApplication(const HelloTriangleApplication& application) = delete;
    HelloTriangleApplication& operator=(const HelloTriangleApplication& application) = delete;
    HelloTriangleApplication(HelloTriangleApplication&& application) = delete;
    HelloTriangleApplication& operator=(HelloTriangleApplication&& application) = delete;
    ~HelloTriangleApplication() = default;

public:
    void Run() {
        InitWindow();
        InitVulkan();
        MainLoop();
        CleanUp();
    }

private:
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const std::string WINDOW_TITLE = "Vulkan";
    GLFWwindow* window_ = nullptr;
    VkInstance instance_ = nullptr;
    const std::vector<const char*> VALIDATION_LAYERS{"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
    VkPhysicalDevice physical_device_ = nullptr;
#if defined(NDEBUG)
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif  // NDEBUG

private:
    void InitWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window_ = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE.c_str(), nullptr, nullptr);
    }

    void InitVulkan() {
        CreateInstance();
        SetupDebugMessenger();
        PickPhysicalDevice();
    }

    void PickPhysicalDevice() {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(instance_, &device_count, nullptr);
        if (device_count == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support.");
        }
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(instance_, &device_count, devices.data());
        for (const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                physical_device_ = device;
                break;
            }
        }
        if (physical_device_ == nullptr) {
            throw std::runtime_error("Failed to find a suitable GPU.");
        }
    }

    static bool IsDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        VkPhysicalDeviceFeatures device_features;
        vkGetPhysicalDeviceFeatures(device, &device_features);
#if defined(__APPLE__)
        return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
#else
        return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && device_features.geometryShader != 0;
#endif
    }

    void CreateInstance() {
        if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available.");
        }

        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "Hello Triangle";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "serenity";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        create_info.pApplicationInfo = &app_info;

        auto extensions = GetRequiredExtensions();
        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
        if (ENABLE_VALIDATION_LAYERS) {
            create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
            create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
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

    void SetupDebugMessenger() {
        if (!ENABLE_VALIDATION_LAYERS) {
            return;
        }
        VkDebugUtilsMessengerCreateInfoEXT create_info;
        PopulateDebugMessengerCreateInfo(create_info);
        if (CreateDebugUtilsMessengerEXT(instance_, &create_info, nullptr, &debug_messenger_) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger.");
        }
    }

    static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger) {
        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        if (func != nullptr) {
            return func(instance, create_info, allocator, debug_messenger);
        }
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator) {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (func != nullptr) {
            func(instance, debug_messenger, allocator);
        }
    }

    static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
        create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = DebugCallback;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data) {
        if (message_severity && message_type && user_data) {
        }
        std::cerr << "Validation layer: " << callback_data->pMessage << std::endl;
        return VK_FALSE;
    }

    [[nodiscard]] std::vector<const char*> GetRequiredExtensions() const {
        uint32_t glfw_extension_count = 0;
        const auto** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::span<const char*> glfw_required_extensions(glfw_extensions, glfw_extension_count);
        std::vector<const char*> extensions(glfw_required_extensions.begin(), glfw_required_extensions.end());
        if (ENABLE_VALIDATION_LAYERS) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        return extensions;
    }

    bool CheckValidationLayerSupport() {
        uint32_t layer_count = 0;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        for (const auto& layer_name : VALIDATION_LAYERS) {
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

    void MainLoop() {
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
        }
    }

    void CleanUp() {
        if (ENABLE_VALIDATION_LAYERS) {
            DestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
        }
        vkDestroyInstance(instance_, nullptr);
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;
    try {
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
