# start cmake with the following command:
#
# mkdir build
# cmake -Bbuild .
# cmake --build build
#
cmake_minimum_required(VERSION 3.7)

project(GuiProject)

find_package(OpenGL REQUIRED)

add_executable(GuiProject
    GlutApplication.cpp
    GuiProject.cpp
    Video.cpp
    common/FileLoader.cpp
    common/Texture.cpp
    common/TextureLoader.cpp
    common/Tokenizer.cpp
    common/camera.cpp
    common/matrix4x4.cpp
    common/quaternion.cpp
    common/vector3.cpp
    geo/Brush.cpp
    geo/Entity.cpp
    geo/MapLoader.cpp
    geo/Scene.cpp
    glad.c
    ui/stb_truetype.cpp
    ui/uiButton.cpp
    ui/uiCheckbox.cpp
    ui/uiClipper.cpp
    ui/uiContainer.cpp
    ui/uiControl.cpp
    ui/uiFont.cpp
    ui/uiFreeContainer.cpp
    ui/uiHorizontalContainer.cpp
    ui/uiLabel.cpp
    ui/uiListbox.cpp
    ui/uiManager.cpp
    ui/uiRender3D.cpp
    ui/uiTextbox.cpp
    ui/uiValuebox.cpp
    ui/uiVerticalContainer.cpp
    userInput.cpp
)

# add dependencies
include(cmake/CPM.cmake)

CPMAddPackage("gh:g-truc/glm#master")
CPMAddPackage("gh:freeglut/freeglut#master")
CPMAddPackage(
    NAME ffmpeg
    URL "https://master.dl.sourceforge.net/project/ffmpeg-windows-builds/release/n4.2.4/ffmpeg-N-95014-gf9f95ceebf-win64-shared.7z?viasf=1"
    DOWNLOAD_ONLY YES
)

if(ffmpeg_ADDED)
    add_library(ffmpeg INTERFACE)
    target_include_directories(ffmpeg INTERFACE ${ffmpeg_SOURCE_DIR}/include)
    target_link_directories(ffmpeg INTERFACE ${ffmpeg_SOURCE_DIR})
endif()

target_include_directories(GuiProject
    PRIVATE
        include
        "${freeglut_SOURCE_DIR}/include"
        "${ffmpeg_SOURCE_DIR}/include"
)

target_link_directories(GuiProject PUBLIC ${ffmpeg_SOURCE_DIR})

target_link_libraries(GuiProject
    PRIVATE
        ${OpenGL_LIBRARIES}
        glm
        freeglut_static
        avcodec
        avutil
        avformat
        swscale
)
