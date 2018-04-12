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
  IbodyInv = glm::mat4(0);
  mObject = object;
  mEngine = object->mEngine;
  mModel = object->mModel;
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    cm += vertex.mPos;
  }
  cm /= mModel->mMeshes.front().mVertices.size();

  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    glm::vec3 r = vertex.mPos;// -cm;
    IbodyInv[0][0] += r.y * r.y + r.z * r.z;
    IbodyInv[1][1] += r.x * r.x + r.z * r.z;
    IbodyInv[2][2] += r.x * r.x + r.y * r.y;
    IbodyInv[0][1] = (IbodyInv[1][0] += -r.x * r.y);
    IbodyInv[0][2] = (IbodyInv[2][0] += -r.x * r.z);
    IbodyInv[1][2] = (IbodyInv[2][1] += -r.y * r.z);
  }
  IbodyInv *= mass;
  IbodyInv = glm::inverse(IbodyInv);
  Update(0);
  deltaQ = q;
  deltaX = x;
  return true;
}

void RigidBody::DebugUpdate()
{
  mEngine->GetGraphicsSystem()->DebugDrawLine(x, x + v);
  mEngine->GetGraphicsSystem()->DebugDrawLine(x, x + w);
}

void RigidBody::Update(float dt)
{
  if (dt < 0) {
    return;
  }
  lastModel = GetModelMatrix();
  glm::mat3 R(glm::normalize(q)); // orientation matrix
  glm::mat3 Iinv = R * IbodyInv * glm::transpose(R);

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

  P = P + force * dt + linearImpulse * mass * dt;
  v = P / mass;
  a = force / mass;

  deltaX = v * dt;// +0.5f * a * dt * dt;
  x = x + deltaX;


  L = L + torque * dt + glm::inverse(Iinv) * angularImpulse * dt;
  w = Iinv * L;
  wp = Iinv * torque;

  glm::quat qdot = 0.5f * glm::cross(glm::quat(0, w), q);
  //glm::quat qdotdot = 0.5f * glm::cross(qdot, glm::quat(0, w)) + glm::cross(q, glm::quat(0, wp));

  deltaQ = qdot * dt;// +0.5f * qdotdot * dt * dt;
  q += deltaQ;

  linearImpulse = glm::vec3(0, 0, 0);
  angularImpulse = glm::vec3(0, 0, 0);
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

void RigidBody::ApplyLinearImpulse(glm::vec3 impulse)
{
  linearImpulse += impulse;
}

void RigidBody::ApplyAngularImpulse(glm::vec3 impulse)
{
  angularImpulse += impulse;
}

void RigidBody::SetState(const RigidBodyData & data)
{
  lastModel = GetModelMatrix();
  deltaQ = data.q;
  deltaQ -= q;

  deltaX = data.x - x;

  x = data.x;
  q = data.q;
  P = data.P;
  L = data.L;
  torque = data.torque;
  force = data.force;
  mass = data.mass;

  glm::mat3 R(glm::normalize(q));
  glm::mat3 I = R * IbodyInv * glm::transpose(R);
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
  return GetModelMatrix() * glm::inverse(lastModel);
}

float RigidBody::GetMassInverse()
{
  return mass != 0.0f ? 1.0f / mass : 0.0f;
}

glm::mat3 RigidBody::GetInertiaTensorInverse()
{
  glm::mat3 R(glm::normalize(q)); // orientation matrix
  glm::mat3 Iinv = R * IbodyInv * glm::transpose(R);
  return Iinv;
}

glm::vec3 RigidBody::GetLinearVelocity()
{
  return v;
}

glm::vec3 RigidBody::GetAngularVelocity()
{
  return w;
}

glm::vec3 RigidBody::GetForces()
{
  return force;
}

glm::vec3 RigidBody::GetTorques()
{
  return torque;
}

glm::vec3 RigidBody::GetLinearImpulse()
{
  return linearImpulse;
}

glm::vec3 RigidBody::GetAngularImpulse()
{
  return angularImpulse;
}


