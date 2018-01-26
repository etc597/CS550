#include "Graphics/GraphicsSystem.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/glm.hpp>

#include "Core/Engine.hpp"
#include "Editor/EditorSystem.hpp"
#include "Utilities.hpp"

static GraphicsSystem* gGraphicsSystem = nullptr;
static bool firstMouse = true;
double lastX = 0, lastY = 0;

void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
  gGraphicsSystem->FramebufferSizeCallback(window, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  gGraphicsSystem->MouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  gGraphicsSystem->ScrollCallback(window, xoffset, yoffset);
}

GraphicsSystem::GraphicsSystem(Engine * engine)
  : mEngine(engine)
  , mEditor(nullptr)
  , mWindow(nullptr)
  , mScreenWidth(800)
  , mScreenHeight(600)
  , mLastFrame(0)
  , mShaders()
{
}

bool GraphicsSystem::Init(EditorSystem * editor)
{
  if (gGraphicsSystem) {
    std::cout << "Attempting to create multiple graphics systems, not allowed" << std::endl;
    return false;
  }
  gGraphicsSystem = this;
  mEditor = editor;

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

  if (mEditor) {
    mEditor->Init(mWindow);
  }
  glfwMakeContextCurrent(mWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glViewport(0, 0, mScreenWidth, mScreenHeight);
  glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
  glfwSetCursorPosCallback(mWindow, mouseCallback);
  glfwSetScrollCallback(mWindow, scrollCallback);

  glEnable(GL_DEPTH_TEST);
  mAssetPath = fs::current_path();
  mAssetPath /= ASSET_PATH;

  mShaders.push_back(Shader(mAssetPath / "shaders", "simple"));

  for (auto& file : fs::directory_iterator(mAssetPath / "models")) {
    auto modelName = file.path().stem();
    mModels.emplace(std::make_pair(modelName.string(), file.path()));
  }

  return true;
}

float GraphicsSystem::Update()
{
  float currentFrame = (float)glfwGetTime();
  float dt = (currentFrame - mLastFrame);

  if (dt > 0.5f) {
    dt = 0.5f;
  }

  ProcessInput(mWindow, dt);

  if (glfwWindowShouldClose(mWindow)) {
    mEngine->Quit();
  }

  glClearColor(0.1f, 0.4f, 0.9f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mShaders[0].UseShaderProgram();

  if (mEditor) {
    mEditor->Update();
  }

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
  if (mEditor) {
    mEditor->Deinit();
    delete mEditor;
  }
}

void GraphicsSystem::FramebufferSizeCallback(GLFWwindow * window, int width, int height)
{
  UNUSED(window);
  mScreenWidth = width;
  mScreenHeight = height;
  glViewport(0, 0, width, height);
}

void GraphicsSystem::MouseCallback(GLFWwindow * window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
  }
  if (mCamera.mMove) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    mCamera.mYaw += xoffset;
    mCamera.mPitch += yoffset;

    glm::vec3 front;
    front.x = cos(glm::radians(mCamera.mPitch)) * cos(glm::radians(mCamera.mYaw));
    front.y = sin(glm::radians(mCamera.mPitch));
    front.z = cos(glm::radians(mCamera.mPitch)) * sin(glm::radians(mCamera.mYaw));
    mCamera.mCameraFront = glm::normalize(front);
  }
  lastX = xpos;
  lastY = ypos;
}

void GraphicsSystem::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
  float fov = mCamera.mFov;
  if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if (fov <= 1.0f)
    fov = 1.0f;
  if (fov >= 45.0f)
    fov = 45.0f;
  mCamera.mFov = fov;
}

void GraphicsSystem::ProcessInput(GLFWwindow * window, float dt)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    mCamera.mMove = true;

  float cameraSpeed = 2.5f * dt; // adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    mCamera.mPosition += cameraSpeed * mCamera.mCameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    mCamera.mPosition -= cameraSpeed * mCamera.mCameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    mCamera.mPosition -= glm::normalize(glm::cross(mCamera.mCameraFront, mCamera.mCameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    mCamera.mPosition += glm::normalize(glm::cross(mCamera.mCameraFront, mCamera.mCameraUp)) * cameraSpeed;
}
