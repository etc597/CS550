#include "Graphics/GraphicsSystem.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
  , mDebug(true)
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
  mShaders.push_back(Shader(mAssetPath / "shaders", "debug"));

  for (auto& file : fs::directory_iterator(mAssetPath / "models")) {
    auto modelName = file.path().stem();
    mModels.emplace(std::make_pair(modelName.string(), file.path()));
  }

  if (!DebugInit()) {
    return false;
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

  Draw();

  DebugDraw();

  if (mEditor) {
    mEditor->Update();
  }

  glfwSwapBuffers(mWindow);
  glfwPollEvents();

  mLastFrame = currentFrame;
  
  return dt > 0.016f ? 0.016f : dt;
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
    firstMouse = false;
  }
  if (mCamera.mMove) {
    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos); // reversed since y-coordinates range from bottom to top

    float sensitivity = 0.125f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    mCamera.mYaw += xoffset;
    mCamera.mPitch += yoffset;

    if (mCamera.mPitch > 89.0f)
      mCamera.mPitch = 89.0f;
    if (mCamera.mPitch < -89.0f)
      mCamera.mPitch = -89.0f;

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
  UNUSED(xoffset);
  if (mCamera.mMove) {
    mCamera.mFov -= (float)yoffset;
  }
}

Model * GraphicsSystem::GetModel(const std::string & name)
{
  auto it = mModels.find(name);
  if (it != mModels.end()) {
    return &it->second;
  }
  return nullptr;
}

bool GraphicsSystem::GetDebug()
{
  return mDebug;
}

void GraphicsSystem::SetDebug(bool val)
{
  mDebug = val;
}

void GraphicsSystem::DebugDrawLine(const glm::vec3& p1, const glm::vec3& p2, bool force /*= false*/)
{
  if (mDebug || force) {
    mDebugLines.push_back(p1);
    mDebugLines.push_back(p2);
  }
}

bool GraphicsSystem::DebugInit()
{
  glGenVertexArrays(1, &mDebugVAO);
  glBindVertexArray(mDebugVAO);

  glGenBuffers(1, &mDebugVBO);
  glBindBuffer(GL_ARRAY_BUFFER, mDebugVBO);
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

  glBindVertexArray(0);
  return true;
}

void GraphicsSystem::ProcessInput(GLFWwindow * window, float dt)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    mCamera.mMove = true;
  else
    mCamera.mMove = false;

  if (mCamera.mMove) {
    float cameraSpeed = 5.0f * dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      mCamera.mPosition += cameraSpeed * mCamera.mCameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      mCamera.mPosition -= cameraSpeed * mCamera.mCameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      mCamera.mPosition -= glm::normalize(glm::cross(mCamera.mCameraFront, mCamera.mCameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      mCamera.mPosition += glm::normalize(glm::cross(mCamera.mCameraFront, mCamera.mCameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      mCamera.mPosition += cameraSpeed * mCamera.mCameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      mCamera.mPosition -= cameraSpeed * mCamera.mCameraUp;
  }

}

void GraphicsSystem::Draw()
{
  mShaders[0].UseShaderProgram();

  glm::mat4 view;
  view = glm::lookAt(mCamera.mPosition, mCamera.mPosition + mCamera.mCameraFront, mCamera.mCameraUp);
  mShaders[0].SetMat4("view", view);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(mCamera.mFov), static_cast<float>(mScreenWidth) / mScreenHeight, 0.1f, 100.0f);
  mShaders[0].SetMat4("projection", projection);

  auto objects = mEngine->GetObjects();

  // Draw objects
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  for (auto& obj : objects) {
    mShaders[0].SetMat4("model", obj.GetModelMatrix());
    mShaders[0].SetVec3("obj_color", obj.mColor);
    obj.mModel->Draw(&mShaders[0]);
  }

  // Draw lines
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for (auto& obj : objects) {
    mShaders[0].SetMat4("model", obj.GetModelMatrix());
    mShaders[0].SetVec3("obj_color", glm::vec3(0));
    obj.mModel->Draw(&mShaders[0]);
  }
}

void GraphicsSystem::DebugDraw()
{
  auto& objects = mEngine->GetObjects();

  for (auto& obj : objects) {
    obj.mRigidBody->DebugUpdate();
  }

  mShaders[1].UseShaderProgram();
  glm::mat4 view;
  view = glm::lookAt(mCamera.mPosition, mCamera.mPosition + mCamera.mCameraFront, mCamera.mCameraUp);
  mShaders[1].SetMat4("view", view);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(mCamera.mFov), static_cast<float>(mScreenWidth) / mScreenHeight, 0.1f, 100.0f);
  mShaders[1].SetMat4("projection", projection);


  //glm::mat4 model ({ 1, 0, 0,
  //                    0, 1, 0,
  //                    0, 0, 1 });
  mShaders[1].SetMat4("model", glm::mat4());
  mShaders[1].SetVec3("obj_color", glm::vec3(1, 0, 0));


  glBindVertexArray(mDebugVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mDebugVBO);
  glBufferData(GL_ARRAY_BUFFER, mDebugLines.size() * sizeof(glm::vec3), mDebugLines.data(), GL_DYNAMIC_DRAW);

  glLineWidth(1.5f);
  glDrawArrays(GL_LINES, 0, mDebugLines.size());
  glBindVertexArray(0);

  mDebugLines.clear();
}
