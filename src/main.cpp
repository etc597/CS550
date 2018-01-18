#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Utilities.hpp"

int screenWidth = 800;
int screenHeight = 600;

float lastFrame = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, WINDOW_NAME, NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window, check your openGL version" << std::endl;
    glfwTerminate();
    return -1;
  }
  ImGui_ImplGlfwGL3_Init(window, true);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  glEnable(GL_DEPTH_TEST);

  // program loop
  while (!glfwWindowShouldClose(window)) {
    ImGui_ImplGlfwGL3_NewFrame();
    float currentFrame = (float)glfwGetTime();
    float dt = (currentFrame - lastFrame);

    if (dt > 0.5f) {
      dt = 0.5f;
    }

    glClearColor(0.1f, 0.4f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    ImGui::Render();

    glfwSwapBuffers(window);
    glfwPollEvents();

    lastFrame = currentFrame;
  }

  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height)
{
  UNUSED(window);
  screenWidth = width;
  screenHeight = height;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
