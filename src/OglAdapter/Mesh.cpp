#include "OglAdapter/Mesh.h"
#include <iostream>

using namespace VanOGL;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    vertices_ = vertices;
    indices_  = indices;
    textures_ = textures;

    setupMesh();
}

// render the mesh
void Mesh::Draw(VanOGL::Shader &shader)
{
    // bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    for (unsigned int i = 0; i < textures_.size(); i++) {
        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures_[i].typeDesc_;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        } else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
        } else if (name == "texture_normal") {
            number = std::to_string(normalNr++);
        } else if (name == "texture_height") {
            number = std::to_string(heightNr++);
        }

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
    }

    // draw mesh
    glBindVertexArray(VAO_);
    if (indices_.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    } else {
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

// initializes all the buffer objects/arrays
void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2
    // array which again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}
