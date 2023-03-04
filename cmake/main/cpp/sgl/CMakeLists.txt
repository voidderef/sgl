project(sgl)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/sgl)
include_directories(${SGL_SRC_MAIN_CPP_DIR}/sgl/attract)
include_directories(${SGL_SRC_MAIN_CPP_DIR}/sgl/boot)

# For KS headers to build the correct paths
if (WIN32)
    add_definitions(-DKS_PLATFORM_WIN32)
else ()
    add_definitions(-DKS_PLATFORM_LINUX)
endif ()

if (WIN32)
    add_definitions(-DSGL_PLATFORM_WIN32)
else ()
    add_definitions(-DSGL_PLATFORM_LINUX)
endif ()

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/attract/AttractLuaBindings.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/attract/AttractScreen.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/attract/DefaultGameSelectorTimer.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/boot/BootScreen.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/error/ErrorScreen.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaAsset.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaDebug.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaFont.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaHelper.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaIOPHelper.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaISPBindings.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaNativeToLuaHelper.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaOSPBindings.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaRenderer.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaSettingsLoader.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaSound.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaSprite.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaState.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaTexture.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaTime.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaToNativeHelper.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/lua/LuaVideo.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/operator/Menu.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/operator/MenuItemText.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/operator/MenuPageText.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/operator/OperatorScreen.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/DataManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/FontManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/GameLauncher.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/Games.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/main.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/OutputAnimation.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/Renderer.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SaveState.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SFXManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SGL.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SGLSettings.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/ShellScript.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SoundRenderer.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/SpriteRenderer.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/TextOutputAnimation.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/TextureManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/VideoDecoder.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/VideoManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/sgl/WorldState.cpp ../../../../src/main/cpp/io/joystick/sdl2/Joystick.cpp ../../../../src/main/cpp/io/joystick/sdl2/Joystick.h ../../../../src/main/cpp/io/joystick/DigitalInputs.h ../../../../src/main/cpp/sgl/attract/DefaultGameSelectorTimer.cpp ../../../../src/main/cpp/sgl/attract/DefaultGameSelectorTimer.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (WIN32)
    # Link stuff staticly
    # Comes with kitchen sink staticly linked
    # Note: This doesn't work anymore if we move commonly shared stuff to
    # a kitchen sink lib
    # set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")

    target_link_libraries(${PROJECT_NAME} ks fmt lua io fmodex SDL2 SDL2_image SDL2_ttf avcodec avformat avutil swscale)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGITREV=${GITREV} -O3 -g")
else ()
    target_link_libraries(${PROJECT_NAME} ks io lua SDL2 SDL2_image SDL2_ttf avcodec avformat avutil swscale pthread dl dw fmodex)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGITREV=${GITREV} -O3 -g")
endif ()