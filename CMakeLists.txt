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
    if (APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework IOKit")
        include_directories(
            "/usr/local/include"
        )
        link_directories(
            "/usr/local/lib"
        )
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

file(GLOB srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB tests RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

include_directories(
    "include"
    "nlohmann"
    "glfw/include"
)

link_directories(
    "glfw/lib"
)

foreach(test IN LISTS tests)
    get_filename_component(testname ${test} NAME_WE)
    add_executable(${testname} ${test} ${srcs})
    if (MSVC)
        target_link_libraries(${testname} vulkan-1 glfw3)
    else()
        target_link_libraries(${testname} vulkan glfw3)
    endif()
endforeach()