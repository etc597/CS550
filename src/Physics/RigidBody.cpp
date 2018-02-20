#include "RigidBody.hpp"

#include "Core/Engine.hpp"
#include "Core/Object.hpp"
#include "Graphics/Model.hpp"
using namespace ELBA;

#include <glm/gtc/matrix_transform.hpp>

glm::mat3 helper_matrix(glm::vec3 a)
{
  return glm::mat3(   0, -a.z,  a.y,
                    a.z,    0, -a.x,
                   -a.y,  a.x,    0);
}

RigidBody::RigidBody()
  : mObject(nullptr)
  , mEngine(nullptr)
  , mModel(nullptr)
  , mass(1.0f)
{
}

RigidBody::RigidBody(const RigidBodyData & data)
  : mObject(nullptr)
  , mEngine(nullptr)
  , mModel(nullptr)
  , x(data.x)
  , q(data.q)
  , P(data.P)
  , L(data.L)
  , torque(data.torque)
  , force(data.force)
  , mass(data.mass)
{
  ApplyForce(force);
  ApplyTorque(torque);
}

bool RigidBody::Init(Object * object)
{
  Ibody = glm::mat4(0);
  mObject = object;
  mEngine = object->mEngine;
  mModel = object->mModel;
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    cm += vertex.mPos;
  }
  cm /= mModel->mMeshes.front().mVertices.size();

  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    glm::vec3 r = vertex.mPos - cm;
    Ibody[0][0] += r.y * r.y + r.z * r.z;
    Ibody[1][1] += r.x * r.x + r.z * r.z;
    Ibody[2][2] += r.x * r.x + r.y * r.y;
    Ibody[0][1] = (Ibody[1][0] += -r.x * r.y);
    Ibody[0][2] = (Ibody[2][0] += -r.x * r.z);
    Ibody[1][2] = (Ibody[2][1] += -r.y * r.z);
  }
  Ibody *= mass;
  Update(0);
  return true;
}

void RigidBody::DebugUpdate()
{
  mEngine->GetGraphicsSystem()->DebugDrawLine(x, x + v);
  mEngine->GetGraphicsSystem()->DebugDrawLine(x, x + w);
}

void RigidBody::Update(float dt)
{
  glm::mat3 R(glm::normalize(q)); // orientation matrix
  glm::mat3 I = R * Ibody * glm::transpose(R);
  glm::mat3 Iinv = glm::inverse(I);

  force = glm::vec3(0);
  torque = glm::vec3(0);

  // take in applied torques
  for (auto& t : mAppliedTorques) {
    torque += t;
  }
  mAppliedTorques.clear();

  // take in applied forces
  for (auto& aF : mAppliedForces) {
    torque += glm::cross(aF.pos - x, aF.force);
    force += aF.force;
  }
  mAppliedForces.clear();

  P = P + force * dt;
  v = P / mass;
  a = force / mass;

  deltaX = v * dt + 0.5f * a * dt * dt;
  x = x + deltaX;


  L = L + torque * dt;
  w = Iinv * L;
  wp = Iinv * torque;

  glm::quat qdot = 0.5f * glm::cross(glm::quat(0, w), q);
  glm::quat qdotdot = 0.5f * glm::cross(qdot, glm::quat(0, w)) + glm::cross(q, glm::quat(0, wp));

  deltaQ = qdot * dt + 0.5f * qdotdot * dt * dt;
  q += deltaQ;
}

void RigidBody::ApplyForce(glm::vec3 force, glm::vec3 pos)
{
  mAppliedForces.push_back(AppliedForce(force, pos));
}

void RigidBody::ApplyForce(glm::vec3 force)
{
  ApplyForce(force, x);
}

void RigidBody::ApplyTorque(glm::vec3 torque)
{
  mAppliedTorques.push_back(torque);
}

void RigidBody::SetState(const RigidBodyData & data)
{
  x = data.x;
  q = data.q;
  P = data.P;
  L = data.L;
  torque = data.torque;
  force = data.force;
  mass = data.mass;

  glm::mat3 R(glm::normalize(q));
  glm::mat3 I = R * Ibody * glm::transpose(R);
  glm::mat3 Iinv = glm::inverse(I);

  v = P / mass;
  a = force / mass;
  w = Iinv * L;
  wp = Iinv * torque;
}

RigidBodyData RigidBody::GetState()
{
  RigidBodyData data;
  data.x = x;
  data.q = q;
  data.P = P;
  data.L = L;
  data.torque = torque;
  data.force = force;
  data.mass = mass;
  return data;
}

glm::mat4 RigidBody::GetModelMatrix()
{
  glm::vec3 scale(1, 1, 1);
  glm::mat4 rotate(glm::normalize(q));

  glm::mat4 model;
  model = glm::translate(model, x);
  model *= rotate;
  model = glm::scale(model, scale);
  return model;
}

glm::vec3 RigidBody::GetPos()
{
  return x;
}

glm::mat4 RigidBody::GetDeltaMatrix()
{
  glm::vec3 scale(1, 1, 1);
  glm::mat4 rotate(deltaQ);

  glm::mat4 delta;
  delta = glm::translate(delta, deltaX);
  delta *= rotate;
  delta = glm::scale(delta, scale);
  return delta;
}


