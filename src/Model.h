#pragma once

#include<iostream>
#include<vector>
#include<list>
#include<string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h> 

#include"Mesh.h"

#include "Profiler.h"

//Model requirements: each (assimp) Mesh MUST have at least Positions

//TODO: models multiple texture coordinates support

class Model
{
public:
    Model(std::string path);

    //copy constructor; at this point it's better to delete it, plus underlying classes are not copyable anyway
    Model(const Model& copy) = delete;

    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Mesh_Texture> textures_loaded;
    
    void loadModel(std::string path);

    void processNode(aiNode* node, const aiScene* scene); //recursive function; to establish relations (parent-child) between model parts (this is not done yet)

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Mesh_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); //texture paths are relative to model folder; absolute paths won't work

    unsigned int textureFromFile(const aiString& path, const std::string& directory);

    static void to_glm_vec3(glm::vec3& destination, aiVector3D& mVertices);
    static void to_glm_vec2(glm::vec2& destination, aiVector3D& mVertices);

};
