#include "Graphics/GraphicsSystem.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Core/Engine.hpp"
#include "Utilities.hpp"

static GraphicsSystem* gGraphicsSystem = nullptr;

void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
  gGraphicsSystem->FramebufferSizeCallback(window, width, height);
}

GraphicsSystem::GraphicsSystem(Engine * engine)
  : mEngine(engine)
  , mWindow(nullptr)
  , mScreenWidth(800)
  , mScreenHeight(600)
  , mLastFrame(0)
{
}

bool GraphicsSystem::Init()
{
  if (gGraphicsSystem) {
    std::cout << "Attempting to create multiple graphics systems, not allowed" << std::endl;
    return false;
  }
  gGraphicsSystem = this;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, WINDOW_NAME, NULL, NULL);
  if (!mWindow) {
    std::cout << "Failed to create GLFW window, check your openGL version" << std::endl;
    glfwTerminate();
    return false;
  }
  ImGui_ImplGlfwGL3_Init(mWindow, true);
  glfwMakeContextCurrent(mWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glViewport(0, 0, mScreenWidth, mScreenHeight);
  glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

  glEnable(GL_DEPTH_TEST);
  return true;
}

float GraphicsSystem::Update()
{
  ImGui_ImplGlfwGL3_NewFrame();
  float currentFrame = (float)glfwGetTime();
  float dt = (currentFrame - mLastFrame);

  if (dt > 0.5f) {
    dt = 0.5f;
  }

  ProcessInput(mWindow);

  if (glfwWindowShouldClose(mWindow)) {
    mEngine->Quit();
  }

  glClearColor(0.1f, 0.4f, 0.9f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui::Render();

  glfwSwapBuffers(mWindow);
  glfwPollEvents();

  mLastFrame = currentFrame;
  
  return dt;
}

void GraphicsSystem::Deinit()
{
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();
  gGraphicsSystem = nullptr;
}

void GraphicsSystem::FramebufferSizeCallback(GLFWwindow * window, int width, int height)
{
  UNUSED(window);
  mScreenWidth = width;
  mScreenHeight = height;
  glViewport(0, 0, width, height);
}

void GraphicsSystem::ProcessInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
