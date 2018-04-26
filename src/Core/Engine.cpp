#include "Engine.hpp"
#include "Editor/EditorSystem.hpp"

#include <iostream>
#include <sstream>

Engine::Engine(int argc, char * argv[])
  : mState(State::Play)
  , mGraphics(this)
  , mPhysics(this)
  , mShouldQuit(false)
{
}

Engine::~Engine()
{
}

bool Engine::Init()
{
  mObjects.reserve(100000); //TODO: Remove this hack and just use uniques

  mJobSystem.Initialize();

  if (!mGraphics.Init(new EditorSystem(this))) {
    return false;
  }

  if (!mPhysics.Init()) {
    return false;
  }

  ObjectData obj;
  RigidBodyData data;

  obj.mName = "Sphere";
  obj.mModelName = "sphere";
  obj.mColor = glm::vec3(0.1f, 0.7f, 0.1f);

  data.x = glm::vec3(0, 0, -0.5f);
  data.mass = 2.0f;

  if (!CreateObject(obj, data)) {
    std::cout << "Failed to create cube object, aborting" << std::endl;
    return false;
  }

  const int NUM_SPHERES = 1000;
  float radius = 250;
  for (unsigned i = 0; i < NUM_SPHERES; ++i)
  {
    data = RigidBodyData();
    std::stringstream stream;
    stream << "Cube" << i;
    obj.mName = stream.str();
    obj.mModelName = "cube";
    obj.mColor = glm::vec3(0.5f + 0.5f * (1.0f / NUM_SPHERES * i), 1.0f / NUM_SPHERES * i, 1.0f - (1.0f / NUM_SPHERES * i));

    data.x = glm::vec3(radius * std::cos(2.0f * glm::pi<float>() / NUM_SPHERES * i), 0.3f, radius * std::sin(2.0f * glm::pi<float>() / NUM_SPHERES * i));
    data.P = -data.x;
    data.P.y = 0;
    data.P = 5.0f * glm::normalize(data.P);
    data.mass = 1.0f;

    if (!CreateObject(obj, data)) {
      std::cout << "Failed to create " << obj.mName << ", aborting" << std::endl;
      return false;
    }
  }
  
  data = RigidBodyData();
  obj.mName = "Cylinder";
  obj.mModelName = "cylinder";
  obj.mColor = glm::vec3(0.1f, 0.4f, 0.4f);
  
  data.x = glm::vec3(1.7f, -0.5f, -1.1f);
  data.mass = 1.5f;
  
  if (!CreateObject(obj, data)) {
    std::cout << "Failed to create cube object, aborting" << std::endl;
    return false;
  }

  Cache();


  return true;
}

void Engine::Run()
{
  while (!ShouldQuit()) 
  {
    Update();
  }
}

void Engine::Quit()
{
  mShouldQuit = true;
}

void Engine::Deinit()
{
  mGraphics.Deinit();
  mPhysics.Deinit();
}

void Engine::Play()
{
  mState = State::Play;
}

void Engine::Pause()
{
  mState = State::Pause;
}

void Engine::Rewind()
{
  mState = State::Rewind;
}

bool Engine::CreateObject(const ObjectData& obj, const RigidBodyData& data)
{
  mObjects.push_back(Object(this));
  return mObjects.back().Init(obj, data);
}

void Engine::DeleteObject(const std::string & objName)
{
  auto it = std::find_if(mObjects.begin(), mObjects.end(), [objName](const Object& obj) {
    if (obj.mName != objName) {
      return false;
    }
    return true;
  });
  if (it != mObjects.end()) {
    it->Deinit();
    mObjects.erase(it);
  }
}

const std::vector<Object>& Engine::GetObjects() const
{
  return mObjects;
}

std::vector<Object>& Engine::GetObjects()
{
  return mObjects;
}

GraphicsSystem * Engine::GetGraphicsSystem()
{
  return &mGraphics;
}

PhysicsSystem * Engine::GetPhysicsSystem()
{
  return &mPhysics;
}

JobSystem * Engine::GetJobSystem()
{
  return &mJobSystem;
}

void Engine::Update()
{
  float dt = mGraphics.Update();

  switch (mState) {
  case State::Pause:
    mPhysics.Update(0);
    break;
  case State::Play:
    mPhysics.Update(dt);
    Cache();
    break;
  case State::Rewind:
    mPhysics.Update(-1);
    RewindUpdate();
    break;
  default:
    std::cout << "Engine in unknown state, aborting" << std::endl;
    Quit();
    break;
  }

  mJobSystem.Update();
}

void Engine::Cache()
{
  FrameData frameData;
  for (auto& obj : mObjects) {
    frameData.mData.insert(std::make_pair(&obj, obj.mRigidBody->GetState()));
  }
  mFrameData.push(frameData);
}

void Engine::RewindUpdate()
{
  if (mFrameData.empty()) {
    mState = State::Pause;
    Cache();
  }
  else {
    auto frameData = mFrameData.top();
    mFrameData.pop();

    for (auto& obj : mObjects) {
      auto it = frameData.mData.find(&obj);
      if (it != frameData.mData.end()) {
        obj.mRigidBody->SetState(it->second);
      }
    }
  }
}

bool Engine::ShouldQuit()
{
  return mShouldQuit;
}
