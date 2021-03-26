#pragma once

#include "OglAdapter/Shader.h"
#include "OglAdapter/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

namespace VanOGL {

class Model {
public:
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false);

    // draws the model, and thus all its meshes
    void Draw(Shader &shader);

public:
    // model data
    std::vector<Mesh> meshes_;
    std::string directory_;
    bool gammaCorrection_;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this
    // process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    void loadMaterialTextures(aiMaterial *mat,
                              aiTextureType type,
                              std::string typeName,
                              std::vector<Texture> &textures);
};

}; // namespace VanOGL
