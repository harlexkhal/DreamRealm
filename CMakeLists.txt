cmake_minimum_required(VERSION 3.8)

project(DreamRealm LANGUAGES CXX)

add_subdirectory(Dependencies/CrunchMath)
add_subdirectory(Dependencies/assimp)
add_subdirectory(Dependencies/glad)
add_subdirectory(Dependencies/glfw)
add_subdirectory(DreamRealm)

# default startup project for Visual Studio
if (MSVC)
 set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DreamRealm)
endif()