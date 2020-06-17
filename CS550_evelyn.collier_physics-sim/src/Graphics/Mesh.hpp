#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/Exporter.hpp>

#include "Shader.hpp"

namespace ELBA
{

  struct Vertex
  {
    glm::vec3 mPos;
    glm::vec3 mNormal;
    glm::vec2 mTexCoords;
  };

  struct Texture
  {
    unsigned int mID;
    std::string mType;
    aiString mPath;
  };

  class Mesh
  {
  public:

    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<Texture> mTextures;

    Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<Texture> aTextures);

    void Draw(Shader *aShader);

  private:

    unsigned int mVAO;
    unsigned int mVBO;
    unsigned int mEBO;

    void SetUpMesh();

  };

}