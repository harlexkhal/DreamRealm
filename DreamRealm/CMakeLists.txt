project(DreamRealm LANGUAGES CXX)

file(GLOB DREAMREALM_SOURCE_FILES
	"src/*.*"
    "src/Resource/*.*")


source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "src" FILES ${DREAMREALM_SOURCE_FILES})
add_executable(DreamRealm ${DREAMREALM_SOURCE_FILES})

target_include_directories(DreamRealm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Assimp_SOURCE_DIR}/include)
target_link_libraries(DreamRealm PUBLIC CrunchMath glfw glad ${OPENGL_LIBRARIES} ${Assimp_BINARY_DIR}/lib/Debug/assimp-vc142-mtd.lib)