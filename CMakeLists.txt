cmake_minimum_required(VERSION 3.23)
project(serenity)

project(serenity LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB src RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/glad/glad.c"
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework OpenGL -framework Cocoa -framework IOKit")
    link_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/mac"
    )
endif()


include_directories(
    "include"
    "include/thirdparty"
    "include/thirdparty/glad"
    "include/thirdparty/glfw"
    "include/thirdparty/spdlog"
    "include/thirdparty/spdlog/cfg"
    "include/thirdparty/spdlog/details"
    "include/thirdparty/spdlog/fmt"
    "include/thirdparty/spdlog/fmt/bundled"
    "include/thirdparty/spdlog/sinks"
)

add_executable(${CMAKE_PROJECT_NAME} ${src})

target_link_libraries(${CMAKE_PROJECT_NAME} glfw3 spdlog)