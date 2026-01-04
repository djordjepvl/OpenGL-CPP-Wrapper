cmake_minimum_required(VERSION 3.16)
project(LearningOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

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
)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

target_include_directories(LearningOpenGL PRIVATE 
    ${DEPENDENCIES_PATH}/GLFW/include
    ${DEPENDENCIES_PATH}/GLAD/include
    ${DEPENDENCIES_PATH}/GLM/include
    ${DEPENDENCIES_PATH}/stb/include

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
)
target_link_directories(LearningOpenGL PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022
)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/assets")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" 
     DESTINATION "${CMAKE_BINARY_DIR}/assets")

target_link_libraries(LearningOpenGL PRIVATE 
    glfw3
    OpenGL::GL
)


