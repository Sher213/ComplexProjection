cmake_minimum_required(VERSION 3.14)

project(UniformProjectionRL 
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Dear ImGui and GLFW via CMake FetchContent for instant GUI support
include(FetchContent)

# Fetch GLFW for windowing and OpenGL context
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Fetch Dear ImGui
FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9.zip
)
FetchContent_MakeAvailable(imgui)

# ImGui source files library target
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)
target_link_libraries(imgui PRIVATE glfw)

# Main Executable
add_executable(UniformProjectionRL
    main.cpp
    Function.h
    ComplexProjection.h
)

target_include_directories(UniformProjectionRL PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${glfw_SOURCE_DIR}/include"
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(UniformProjectionRL PRIVATE glfw imgui)

# Link OpenGL on Windows/Linux/Mac
if(WIN32)
    target_link_libraries(UniformProjectionRL PRIVATE opengl32)
elseif(APPLE)
    find_library(APPLE_OPENGL_FRAMEWORK OpenGL)
    target_link_libraries(UniformProjectionRL PRIVATE ${APPLE_OPENGL_FRAMEWORK})
else()
    target_link_libraries(UniformProjectionRL PRIVATE GL)
endif()