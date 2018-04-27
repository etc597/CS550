#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "fs.hpp"

namespace ELBA
{
  class Shader
  {
  public:

    // read in shader source code and build shader
    Shader(const fs::path& path, const char * name);

    // activate the shader
    void UseShaderProgram();

    // utility uniform functions
    void SetBool(const std::string &name, bool value);
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);
    void SetMat4(const std::string& name, const glm::mat4& value);
    void SetVec3(const std::string& name, const glm::vec3& value);

    unsigned int GetShaderProgram() const;
    unsigned int GetVertShader() const;
    unsigned int GetFragShader() const;

  private:
    unsigned int mShaderProgram;
    unsigned int mVertShader;
    unsigned int mFragShader;

    // reads in a shader from file
    const GLchar* ReadShader(const fs::path& filename);
  };
}