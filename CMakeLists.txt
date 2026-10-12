cmake_minimum_required(VERSION 3.16)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/GLFW)

add_executable(LearningOpenGL
    src/glad.c
    src/config.h
    src/main.cpp
    src/fileIO.h
    src/input.h
    src/input.cpp

    src/app/app.h
    src/app/app.cpp
    src/app/shader.h
    src/app/shader.cpp
    src/app/VBO.h
    src/app/VBO.cpp
    src/app/VAO.h
    src/app/VAO.cpp
    src/app/EBO.h
    src/app/EBO.cpp
    src/app/texture.h
    src/app/texture.cpp
    src/app/camera.h
    src/app/camera.cpp
    src/app/mesh.h
    src/app/mesh.cpp
    src/app/Object3D.h
    src/app/Object3D.cpp
    src/app/material.h
    src/app/material.cpp
    src/app/skybox.h
    src/app/skybox.cpp
    src/app/shadowMap.h
    src/app/shadowMap.cpp
)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

# GLFW's include path now comes from the glfw target automatically
target_include_directories(LearningOpenGL PRIVATE
    ${DEPENDENCIES_PATH}/GLAD/include
    ${DEPENDENCIES_PATH}/GLM/include
    ${DEPENDENCIES_PATH}/stb/include
    ${DEPENDENCIES_PATH}/json/include
    ${DEPENDENCIES_PATH}/tinygltf/include

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
)

# Copy assets next to the built exe (re-copies on every build)
add_custom_command(TARGET LearningOpenGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:LearningOpenGL>/assets
)

target_link_libraries(LearningOpenGL PRIVATE
    glfw
    OpenGL::GL
)

if(MINGW)
    target_link_options(LearningOpenGL PRIVATE -static)
endif()