#include "LearnOpenGL/trivial.hpp"

int main()
{
    prepare();

    glEnable(GL_DEPTH_TEST);

    Shader ourShader;
    ourShader.load(FileSystem::getAbsPath("shader/LearnOpenGL/3_ModelLoading/model.vs").c_str(),
                   FileSystem::getAbsPath("shader/LearnOpenGL/3_ModelLoading/model.fs").c_str());

    auto renderLoop = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

        ourShader.use();

        ourShader.setMat4("projection", mainCamera.getProjectionMat());
        ourShader.setMat4("view", mainCamera.getViewMat());

        mainCamera.updateCameraTimestamp();
    };

    GlfwMgr::instance().run(renderLoop);

    return 0;
}
