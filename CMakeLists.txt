cmake_minimum_required(VERSION 3.31)
project(supermemo VERSION 0.1.0 LANGUAGES CXX)

# C++ compile flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

# Directories
set(IMGUI_SRC_DIR libs/imgui)
set(LIB_SFML_PATH libs/sfml)
set(LIB_DIR libs)
set(INCLUDE_DIR includes)
set(SRC_DIR src)

set(BUILD_SHARED_LIBS ON)

# Load OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

# Load SFML
find_package(
    SFML 2.6.1 COMPONENTS REQUIRED
    system
    window
    graphics
    PATHS ${LIB_SFML_PATH}
)

# Load source and header files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*.hpp)

# Load ImGUI src files
set(
    IMGUI_LIB_SRC
    ${IMGUI_SRC_DIR}/imgui.cpp
    ${IMGUI_SRC_DIR}/imgui_draw.cpp
    ${IMGUI_SRC_DIR}/imgui_tables.cpp
    ${IMGUI_SRC_DIR}/imgui_widgets.cpp
    ${IMGUI_SRC_DIR}/imgui-SFML.cpp
)

# Include directories
include_directories(
    ${LIB_DIR}
    ${INCLUDE_DIR}
)

# Executable
add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${LIBS_SRC}
    ${IMGUI_LIB_SRC}
)

# Include directory to target
target_include_directories(
    ${CMAKE_PROJECT_NAME} PRIVATE
    ${IMGUI_SRC_DIR}
)

# Link libraries to target
target_link_libraries(
    ${CMAKE_PROJECT_NAME} PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    dl
    ${OPENGL_LIBRARY}
)
