#include "OglAdapter/Model.h"
#include "OglAdapter/TextureMgr.h"

#include <iostream>

using namespace VanOGL;

Model::Model(std::string const &path, bool gamma) : gammaCorrection_(gamma)
{
    loadModel(path);
}

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes_.size(); i++) {
        meshes_[i].Draw(shader);
    }
}

void Model::loadModel(std::string const &path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    // aiProcess_FlipUVs maybe needed
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory_ = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vec3;
        vec3.x          = mesh->mVertices[i].x;
        vec3.y          = mesh->mVertices[i].y;
        vec3.z          = mesh->mVertices[i].z;
        vertex.Position = vec3;
        // normals
        if (mesh->HasNormals()) {
            vec3.x        = mesh->mNormals[i].x;
            vec3.y        = mesh->mNormals[i].y;
            vec3.z        = mesh->mNormals[i].z;
            vertex.Normal = vec3;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec2;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec2.x           = mesh->mTextureCoords[0][i].x;
            vec2.y           = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec2;
            // tangent
            vec3.x         = mesh->mTangents[i].x;
            vec3.y         = mesh->mTangents[i].y;
            vec3.z         = mesh->mTangents[i].z;
            vertex.Tangent = vec3;
            // bitangent
            vec3.x           = mesh->mBitangents[i].x;
            vec3.y           = mesh->mBitangents[i].y;
            vec3.z           = mesh->mBitangents[i].z;
            vertex.Bitangent = vec3;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> textures;
    loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", textures);
    loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", textures);

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(aiMaterial *mat,
                                 aiTextureType type,
                                 std::string typeName,
                                 std::vector<Texture> &textures)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filename;
        filename.append(directory_).append("/").append(str.C_Str());
        auto id = TextureMgr::instance().load(filename, typeName);
        if (id == 0) {
            continue;
        }

        Texture tex{id, typeName};
        textures.emplace_back(std::move(tex));
    }
}
