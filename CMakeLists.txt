cmake_minimum_required(VERSION 3.20)

project(FunctionalSpace
    VERSION 1.0
    LANGUAGES CXX
)

# --------------------------------------------------
# C++ Standard
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# --------------------------------------------------
# Dependencies
# --------------------------------------------------

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(
    external/glfw
)


# --------------------------------------------------
# OpenGL
# --------------------------------------------------

find_package(OpenGL REQUIRED)


# --------------------------------------------------
# Dear ImGui
# --------------------------------------------------

set(IMGUI_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
)

add_library(imgui STATIC

    # Core
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    # GLFW + OpenGL3 backend
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC

    ${IMGUI_DIR}

    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC

    glfw
    OpenGL::GL
)


# --------------------------------------------------
# ImPlot
# --------------------------------------------------

set(IMPLOT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/external/implot
)

add_library(implot STATIC

    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
)

target_include_directories(implot PUBLIC

    ${IMPLOT_DIR}

    ${IMGUI_DIR}
)

target_link_libraries(implot PUBLIC

    imgui
)


# --------------------------------------------------
# Your application
# --------------------------------------------------

add_executable(FunctionalSpace

    src/main.cpp
)


# --------------------------------------------------
# Include directories for your application
# --------------------------------------------------

target_include_directories(FunctionalSpace PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# --------------------------------------------------
# Link everything together
# --------------------------------------------------

target_link_libraries(FunctionalSpace PRIVATE

    implot
    imgui
    glfw
    OpenGL::GL
)


# --------------------------------------------------
# Windows-specific settings
# --------------------------------------------------

if(WIN32)

    target_compile_definitions(FunctionalSpace PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )

endif()


# --------------------------------------------------
# Output directory
# --------------------------------------------------

set_target_properties(FunctionalSpace PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin

)
