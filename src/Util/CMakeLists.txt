set(MODULE_NAME util)

set(MODULE_DEPENDENCY
    ${PROJECT_DEPENDENCY}
    assimp
    nanogui
    )

file(GLOB MODULE_SRC
    "${CMAKE_CURRENT_LIST_DIR}/*.c*"
    )

add_library(${MODULE_NAME} STATIC ${MODULE_SRC})
target_link_libraries(${MODULE_NAME} ${MODULE_DEPENDENCY})
