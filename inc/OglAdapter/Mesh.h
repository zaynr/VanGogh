#pragma once

#include "OglAdapter/Shader.h"
#include "OglAdapter/BasicData.h"

#include <string>
#include <vector>

namespace VanOGL {

class Mesh {
public:
    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    // render the mesh
    void Draw(VanOGL::Shader &shader);

public:
    // mesh Data
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<Texture> textures_;
    GLuint VAO_;

private:
    // initializes all the buffer objects/arrays
    void setupMesh();

private:
    // render data
    GLuint VBO_;
    GLuint EBO_;
};

}; // namespace VanOGL
