#include <LearnOpenGL/trivial.hpp>

int main()
{
    prepare();

    glEnable(GL_DEPTH_TEST);

    auto mshader = ShaderMgr::instance().getShader("model");
    auto mmodel = ModelMgr::instance().getModel("mary");

    unsigned int globShadow = glGetUniformBlockIndex(mshader->getID(), "Matrices");
    glUniformBlockBinding(mshader->getID(), globShadow, 0);

    auto renderLoop = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        updateGlobalCamera();

        mshader->use();

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model =
            glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene, so scale it down
        mshader->setMat4("model", model);
        mmodel->Draw(*mshader);

        mainCamera.updateCameraTimestamp();
    };

    GlfwMgr::instance().run(renderLoop);

    return 0;
}
