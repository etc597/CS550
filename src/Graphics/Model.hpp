#pragma once

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Mesh.hpp"

namespace ELBA
{

  class Model
  {
  public:

    Model(fs::path aPath);

    void Draw(Shader *aShader);

  private:

    std::vector<Mesh> mMeshes;

    std::vector<Texture> mLoadedTextures;

    std::string mDirectory;

    void LoadModel(std::string aPath);
    void ProcessNode(aiNode *aNode, const aiScene *aScene);
    Mesh ProcessMesh(aiMesh *aMesh, const aiScene *aScene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial *aMat, aiTextureType aType, std::string aTypeName);

    unsigned int LoadBMP(const char *aFile, std::string aDir);

    unsigned int LoadTexture(const char *aFile, std::string aDir);

  };

}