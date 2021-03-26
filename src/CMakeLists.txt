set(LearnOGLDemos
    1_GettingStart
    2_Lightening
    3_ModelLoading
    4_AdvancedOpenGL
    5_AdvancedLighting
    6_PBR
    7_InPractice
    )

set(MODULE_DEPENDENCY
    ${PROJECT_DEPENDENCY}
    assimp
    ogl_adapter
    shape
    util
    )
foreach(Demo ${LearnOGLDemos})
    file(GLOB SOURCE
        "${PROJECT_SOURCE_DIR}/src/LearnOpenGL/${Demo}/*.cpp"
        )
    foreach(Src ${SOURCE})
        string(REGEX REPLACE "[/|\\]" ";" SrcNameLst ${Src})
        list(LENGTH SrcNameLst LstLen)
        math(EXPR LstLst "${LstLen} - 1")
        list(GET SrcNameLst ${LstLst} SrcName)
        string(REGEX REPLACE ".cpp" "" SrcName ${SrcName})

        set(Name "${Demo}_${SrcName}")
        add_executable(${Name} ${Src})
        target_link_libraries(${Name} ${MODULE_DEPENDENCY})
    endforeach(Src)
endforeach(Demo)

add_subdirectory(OglAdapter)
add_subdirectory(Shape)
add_subdirectory(Util)
add_subdirectory(Engine)
