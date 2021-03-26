#include <LearnOpenGL/trivial.hpp>

int main()
{
    prepare();

    glEnable(GL_DEPTH_TEST);

    auto debugShader  = ShaderMgr::instance().getShader("1_shadow_debug");
    auto shadowShader = ShaderMgr::instance().getShader("1_shadow_map");
    auto shadowDepShader = ShaderMgr::instance().getShader("1_shadow_dep");
    auto modelMary    = ModelMgr::instance().getModel("mary");

    shadowShader->use();
    shadowShader->setInt("texture_diffuse1", 0);
    shadowShader->setInt("shadowMap", 1);
    debugShader->use();
    debugShader->setInt("depthMap", 0);

    unsigned int globShadow = glGetUniformBlockIndex(shadowShader->getID(), "Matrices");
    glUniformBlockBinding(shadowShader->getID(), globShadow, 0);

    const int shadowSize = 2048;
    FrameBuffer shadowFrame(shadowSize, shadowSize);
    shadowFrame.configureDepthOnly();

    glm::vec3 lightPos(-4.0f, 4.0f, -3.0f);
    glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 40.0f;
    lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)shadowSize / (GLfloat)shadowSize, near_plane, far_plane);
    // lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    shadowDepShader->use();
    shadowDepShader->setMat4("model", glm::mat4(1.0f));
    shadowDepShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    shadowShader->use();
    shadowShader->setVec3("lightPos", lightPos);
    shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

    auto depthMap = shadowFrame.getBuff(GL_DEPTH_ATTACHMENT);

    auto scene = [&](Shader &shad) {
        drawFloor(shad);

        glm::mat4 model = glm::mat4(1.0f);
        model =
            glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shad.setMat4("model", model);
        modelMary->Draw(shad);
    };

    auto renderLoop = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateGlobalCamera();

        shadowDepShader->use();
        glViewport(0, 0, shadowSize, shadowSize);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFrame.getID());
        glClear(GL_DEPTH_BUFFER_BIT);
        scene(*shadowDepShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto &gi = GlobalConfig::instance();
        glViewport(0, 0, gi.getScreenWidth()*2, gi.getScreenHeight()*2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        shadowShader->use();
        // set light uniforms
        shadowShader->setVec3("viewPos", mainCamera.getPos());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        scene(*shadowShader);

        // debugShader->use();
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, depthMap);
        // renderQuad();

        mainCamera.updateCameraTimestamp();
    };

    GlfwMgr::instance().run(renderLoop);

    return 0;
}
