project(VanGogh C CXX)
cmake_minimum_required(VERSION 3.18)

if (POLICY CMP0058)
  cmake_policy(SET CMP0058 NEW)
endif()

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ext/nanogui/src")
  message(FATAL_ERROR "The NanoGUI dependency repositories (nanogui, etc.) are missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by calling \"git submodule update --init --recursive\"")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# compile setting
set(CXX_FLAGS_PROJECT "-Wall -fPIC")
set(MODULE_LINKER_FLAGS_PROJECT)
set(SHARED_LINKER_FLAGS_PROJECT)
set(STATIC_LINKER_FLAGS_PROJECT)
set(EXE_LINKER_FLAGS_PROJECT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS_PROJECT}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${MODULE_LINKER_FLAGS_PROJECT}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SHARED_LINKER_FLAGS_PROJECT}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${STATIC_LINKER_FLAGS_PROJECT}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXE_LINKER_FLAGS_PROJECT}")

# configure
set(NANOGUI_BUILD_EXAMPLE OFF)
set(NANOGUI_BUILD_PYTHON OFF)
set(NANOGUI_INSTALL OFF)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/ext/nanogui "ext_build/nanogui")
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/ext/assimp "ext_build/assimp")

# ext
set(VANGOGH_EXTRA_LIBS "")
set(VANGOGH_EXTRA_DEFS "")
set(VANGOGH_EXTRA_INCS "")

# plat
if (WIN32)
    list(APPEND VANGOGH_EXTRA_LIBS opengl32)
else()
    list(APPEND VANGOGH_EXTRA_LIBS pthread)
endif()

# include path
configure_file(${CMAKE_HOME_DIRECTORY}/configuration/root_directory.h.in configuration/root_directory.h)
set(PROJECT_INC_DIR
    ${CMAKE_HOME_DIRECTORY}/inc
    ${CMAKE_BINARY_DIR}/configuration
    ${CMAKE_HOME_DIRECTORY}/ext/nanogui/include
    ${CMAKE_HOME_DIRECTORY}/ext/nanogui/ext/glfw/include
    ${CMAKE_HOME_DIRECTORY}/ext/nanogui/ext/eigen
    ${CMAKE_HOME_DIRECTORY}/ext/assimp/include
    ${VANGOGH_EXTRA_INCS}
    )

# CMAKE_SOURCE_DIR
include_directories(${PROJECT_INC_DIR})

# ld path
set(PROJECT_LINK_DIR
    ${CMAKE_HOME_DIRECTORY}/lib
    ${CMAKE_HOME_DIRECTORY}/lib/windows
    /usr/local/lib
    )

link_directories(${PROJECT_LINK_DIR})

# ld lib
set(PROJECT_DEPENDENCY
    ${VANGOGH_EXTRA_LIBS}
    )

# output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/output/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/output/lib)

add_subdirectory(src)
