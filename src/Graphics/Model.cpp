#include <iostream>
#include <fstream>
#include <stb_image_aug.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL.h>

#include "Model.hpp"


namespace ELBA
{
  Model::Model(char * aPath)
  {
    LoadModel(aPath);
  }

  void Model::Draw(Shader *aShader)
  {
    for (unsigned int i = 0; i < mMeshes.size(); ++i)
    {
      mMeshes[i].Draw(aShader);
    }
  }

  void Model::LoadModel(std::string aPath)
  {
    Assimp::Importer import;

    const aiScene *scene = import.ReadFile(aPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::cout << "Assimp Error: " << import.GetErrorString() << std::endl;
      return;
    }

    mDirectory = aPath.substr(0, aPath.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

  }

  void Model::ProcessNode(aiNode * aNode, const aiScene * aScene)
  {
    for (unsigned int i = 0; i < aNode->mNumMeshes; ++i)
    {
      aiMesh *mesh = aScene->mMeshes[aNode->mMeshes[i]];
      mMeshes.push_back(ProcessMesh(mesh, aScene));
    }

    for (unsigned int i = 0; i < aNode->mNumChildren; ++i)
    {
      ProcessNode(aNode->mChildren[i], aScene);
    }
  }

  Mesh Model::ProcessMesh(aiMesh * aMesh, const aiScene * aScene)
  {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;
    std::vector<Texture> texs;

    for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
    {
      Vertex vertex;

      // position
      vertex.mPos.x = aMesh->mVertices[i].x;
      vertex.mPos.y = aMesh->mVertices[i].y;
      vertex.mPos.z = aMesh->mVertices[i].z; 

      // normal
      vertex.mNormal.x = aMesh->mNormals[i].x;
      vertex.mNormal.y = aMesh->mNormals[i].y;
      vertex.mNormal.z = aMesh->mNormals[i].z;

      // texture coordinates
      if (aMesh->mTextureCoords[0])
      {
        vertex.mTexCoords.x = aMesh->mTextureCoords[0][i].x;
        vertex.mTexCoords.y = aMesh->mTextureCoords[0][i].y;
      }
      else
      {
        vertex.mTexCoords = glm::vec2(0.0f, 0.0f);
      }

      verts.push_back(vertex);
    }

    // indices
    for (unsigned int i = 0; i < aMesh->mNumFaces; ++i)
    {
      aiFace face = aMesh->mFaces[i];

      for (unsigned int j = 0; j < face.mNumIndices; ++j)
      {
        inds.push_back(face.mIndices[j]);
      }
    }

    aiMaterial *material = aScene->mMaterials[aMesh->mMaterialIndex];

    // store diffuse maps
    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    texs.insert(texs.end(), diffuseMaps.begin(), diffuseMaps.end());

    // store specular maps
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    texs.insert(texs.end(), specularMaps.begin(), specularMaps.end());

    // store normal maps
    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    texs.insert(texs.end(), normalMaps.begin(), normalMaps.end());
    
    // store height maps
    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    texs.insert(texs.end(), heightMaps.begin(), heightMaps.end());
    

    return Mesh(verts, inds, texs);
  }

  std::vector<Texture> Model::LoadMaterialTextures(aiMaterial * aMat, aiTextureType aType, std::string aTypeName)
  {

    std::vector<Texture> texs;

    for (unsigned int i = 0; i < aMat->GetTextureCount(aType); ++i)
    {
      aiString str;
      aMat->GetTexture(aType, i, &str);

      bool alreadyLoaded = false;

      for (unsigned int j = 0; j < mLoadedTextures.size(); ++j)
      {
        if (std::strcmp(mLoadedTextures[j].mPath.C_Str(), str.C_Str()) == 0)
        {
          texs.push_back(mLoadedTextures[j]);
          alreadyLoaded = true;
          break;
        }
      }

      if (!alreadyLoaded)
      {
        Texture texture;
        
        texture.mID = LoadTexture(str.C_Str(), mDirectory);
        texture.mType = aTypeName;
        texture.mPath = str;
        texs.push_back(texture);
        mLoadedTextures.push_back(texture);
      }
    }
    return texs;
  }

  unsigned int Model::LoadBMP(const char * aFile, std::string aDir)
  {
    std::string fullPath = aDir + "/" + aFile;

    char header[54] = { '\0' };
    unsigned int dataPos = 0;
    unsigned int width, height = 0;
    unsigned int imageSize = 0;
    char *data = nullptr;

    std::ifstream file(fullPath);

    if (file.is_open())
    {
      file.read(header, 54);

      if (header[0] != 'B' || header[1] != 'M')
      {
        std::cout << "Not a BMP File" << std::endl;
        return 0;
      }

      dataPos = *(int*)&(header[0x0A]);
      width = *(int*)&(header[0x12]);
      height = *(int*)&(header[0x16]);
      imageSize = *(int*)&(header[0x22]);

      if (imageSize == 0)
      {
        imageSize = width * height * 3;
      }

      if (dataPos == 0)
      {
        dataPos = 54;
      }

      data = new char[imageSize];

      file.read(data, imageSize - 54);
      file.close();


      unsigned int texID;
      glGenTextures(1, &texID);

      glBindTexture(GL_TEXTURE_2D, texID);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

      return texID;
    }
    else
    {
      std::cout << "BMP Texture could not be loaded." << std::endl;
    }

  }

  unsigned int Model::LoadTexture(const char * aFile, std::string aDir)
  {
    std::string fullPath = aDir + "/" + aFile;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else if (nrComponents == 4)
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    }
    else
    {
      std::cout << "Texture failed to load at path: " << fullPath << std::endl;
      stbi_image_free(data);
    }

    return textureID;
  }
}


