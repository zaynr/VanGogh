set(MODULE_NAME VanGogh)

set(MODULE_DEPENDENCY
    ${PROJECT_DEPENDENCY}
    assimp
    ogl_adapter
    util
    nanogui
    )

file(GLOB MODULE_SRC
    "${CMAKE_CURRENT_LIST_DIR}/*.c*"
    )

add_executable(${MODULE_NAME} ${MODULE_SRC})
target_link_libraries(${MODULE_NAME} ${MODULE_DEPENDENCY})
