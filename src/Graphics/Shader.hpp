#pragma once

#include <string>

#include <glad/glad.h>

namespace ELBA
{
  class Shader
  {
  public:

    // read in shader source code and build shader
    Shader(const char *vertPath, const char *fragPath);

    // activate the shader
    void UseShaderProgram();

    // utility uniform functions
    void SetBool(const std::string &name, bool value);
    void SetInt(const std::string &name, int value);
    void SetFloat(const std::string &name, float value);

    unsigned int GetShaderProgram() const;
    unsigned int GetVertShader() const;
    unsigned int GetFragShader() const;

  private:
    unsigned int mShaderProgram;
    unsigned int mVertShader;
    unsigned int mFragShader;

    // reads in a shader from file
    const GLchar* ReadShader(const std::string &filename);
  };
}