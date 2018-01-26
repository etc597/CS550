#include "RigidBody.hpp"

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
  : mModel(nullptr)
  , mass(1.0f)
{
}

RigidBody::RigidBody(const RigidBodyData & data)
  : mModel(nullptr)
  , x(data.x)
  , q(data.q)
  , P(data.P)
  , L(data.L)
  , torque(data.torque)
  , force(data.force)
  , mass(data.mass)
{
}

bool RigidBody::Init(Object * object)
{
  mModel = object->mModel;
  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    cm += vertex.mPos;
  }
  cm /= mModel->mMeshes.front().mVertices.size();

  for (auto& vertex : mModel->mMeshes.front().mVertices) {
    glm::vec3 r = vertex.mPos;
    Ibody[0][0] += r.y * r.y + r.z * r.z;
    Ibody[1][1] += r.x * r.x + r.z * r.z;
    Ibody[1][1] += r.x * r.x + r.y * r.y;
    Ibody[0][1] = Ibody[1][0] = -r.x * r.y;
    Ibody[0][2] = Ibody[2][0] = -r.x * r.z;
    Ibody[1][2] = Ibody[2][1] = -r.y * r.z;
  }
  Ibody *= mass;
  return true;
}

void RigidBody::Update(float dt)
{
  glm::mat3 R(glm::normalize(q)); // orientation matrix
  glm::mat3 Iinv = R * glm::inverse(Ibody) * glm::transpose(R);

  // take in applied torques
  for (auto& t : mAppliedTorques) {
    torque += t;
  }

  // take in applied forces
  for (auto& aF : mAppliedForces) {
    torque += glm::cross(aF.pos - x, aF.force);
    force += aF.force;
  }

  //for (auto& vertex : mModel->mMeshes.front().mVertices) {
  //  glm::vec3 r = R * vertex.mPos + x; // pos of particle in world space
  //  torque += glm::cross(r - x, force);
  //}

  P = P + force * dt;
  v = P / mass;
  a = force / mass;

  L = L + torque * dt;
  w = Iinv * L;
  wp = Iinv * torque;

  x = x + v * dt + 0.5f * a * dt * dt;

  glm::quat qdot = 0.5f * glm::cross(glm::quat(0, w), q);
  glm::quat qdotdot = 0.5f * glm::cross(qdot, glm::quat(0, w)) + glm::cross(q, glm::quat(0, wp));
  q += qdot * dt + 0.5f * qdotdot * dt * dt;

  force = glm::vec3(0);
  torque = glm::vec3(0);
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

glm::mat4 RigidBody::GetModelMatrix()
{
  //glm::vec3 scale(0.2f, 0.2f, 0.2f);
  glm::mat4 rotate(glm::normalize(q));

  glm::mat4 model;
  //model = glm::scale(model, scale);
  model = rotate;
  model = glm::translate(model, x);
  return model;
}


