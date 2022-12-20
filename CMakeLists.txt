cmake_minimum_required(VERSION 3.15)
project(serenity LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if (MSVC)
    add_compile_options(/W4 /WX)
    include_directories(
        "D:/VulkanSDK/1.3.236.0/Include"
    )
    link_directories(
        "D:/VulkanSDK/1.3.236.0/Lib"
    )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set(glm_DIR glm/cmake/glm)
find_package(glm REQUIRED)

add_subdirectory(glm)

file(GLOB srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories(
    "glfw/include"
)

link_directories(
    "glfw/lib"
)

foreach(src IN LISTS srcs)
    get_filename_component(srcname ${src} NAME_WE)
    add_executable(${srcname} ${src})
    target_link_libraries(${srcname} vulkan-1 glfw3 glm::glm)
endforeach()
