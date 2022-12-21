cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# generate compile_commands.json
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
set(CMAKE_SYSTEM_NAME Windows)

project(learn_opengl
  LANGUAGES CXX
)

set(WINDOW_NAME "first_opengl_example")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

# get all cpp source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(
  ${PROJECT_NAME}
  ${SRC_FILES}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
  WINDOW_NAME="${WINDOW_NAME}"
  WINDOW_WIDTH=${WINDOW_WIDTH}
  WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE /usr/x86_64-w64-mingw32/include
)

target_compile_features(
  ${PROJECT_NAME}
  PRIVATE cxx_std_20
)

target_link_options(
  ${PROJECT_NAME}
  PRIVATE
  -static
  -static-libstdc++
  -static-libgcc
)

include(CPM.cmake)
CPMAddPackage("gh:gabime/spdlog#v1.x")
CPMAddPackage("gh:glfw/glfw#3.3.3")
CPMAddPackage("gh:Dav1dde/glad#v0.1.34")
CPMAddPackage("gh:g-truc/glm#0.9.9.8")
CPMAddPackage(
  NAME stb
  GITHUB_REPOSITORY nothings/stb
  GIT_TAG HEAD
)

if (stb_ADDED)
  add_library(stb INTERFACE IMPORTED)
  target_include_directories(stb INTERFACE "${stb_SOURCE_DIR}")
endif()

CPMAddPackage(
  NAME imgui
  GITHUB_REPOSITORY ocornut/imgui
  GIT_TAG v1.82
)

if (imgui_ADDED)
  file(GLOB IMGUI_SRC_FILES "${imgui_SOURCE_DIR}/*.cpp")
  add_library(imgui 
    ${IMGUI_SRC_FILES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)
  target_link_libraries(imgui imm32 glad glfw)
  target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
endif()

target_link_libraries(
  ${PROJECT_NAME}
  stdc++
  gdi32
  spdlog::spdlog
  glfw
  glad
  glm
  stb
  imgui
)