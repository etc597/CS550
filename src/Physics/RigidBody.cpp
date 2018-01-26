#include "RigidBody.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

PhysicsData::PhysicsData()
{
}

PhysicsData::PhysicsData(glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, glm::vec3 impulse)
  : mPosition(position)
  , mVelocity(velocity)
  , mAcceleration(acceleration)
{
}

RigidBody::RigidBody()
  : mLinear()
  , mAngular()
{
}

void RigidBody::Init(Object * object, RigidBodyData data /* = RigidBodyData()*/)
{
}

void RigidBody::Update(float dt)
{
  // Linear Update
  // TODO: Incorporate impulses

  // Linear is easy as the force in a single frame is constant

  glm::vec3 force(0);
  glm::vec3 torque(0);
  for (auto& f : mForces) {
    force += f.mForce;
    torque += glm::cross(f.mPosition, f.mForce);
  }

  mLinear.mAcceleration = force / mMass;
  mLinear.mPosition = mLinear.mPosition + mLinear.mVelocity * dt + mLinear.mAcceleration * dt * dt * 0.5f;
  mLinear.mVelocity = mLinear.mVelocity + mLinear.mAcceleration * dt;

  glm::quat q0(mAngular.mPosition);
  glm::quat qDot0 = 0.5f * glm::cross(q0, glm::quat(0, mAngular.mVelocity));
  auto k1 = qDotDot(q0, qDot0, torque);
  auto k2 = qDotDot(q0 + dt * 0.5f * qDot0 + dt * dt * 0.125f * k1, qDot0 + dt * 0.5f * k1, torque);
  auto k3 = qDotDot(q0 + dt * 0.5f * qDot0 + dt * dt * 0.125f * k2, qDot0 + dt * 0.5f * k2, torque);
  auto k4 = qDotDot(q0 + dt * qDot0 + dt * dt * 0.5f * k3, qDot0 + dt * k3, torque);

  glm::quat q1 = q0 + dt * qDot0 + dt * dt / 6.0f *(k1 + k2 + k3);
  glm::quat qDot1 = qDot0 + dt / 6.0f * (k1 + 2.0f * k2 + 2.0f * k3 + k4);

  mAngular.mVelocity = 2.0f * glm::eulerAngles(glm::cross(glm::conjugate(q1), qDot1));

  mForces.clear();
}

glm::quat RigidBody::qDotDot(glm::quat q, glm::quat qDot, glm::vec3 torque)
{
  glm::quat qBar = glm::conjugate(q);
  glm::quat qBarCrossQDot = glm::cross(qBar, qDot);
  glm::vec3 euler = glm::eulerAngles(qBarCrossQDot);
  return glm::cross(qDot, qBarCrossQDot)
    + 0.5f * glm::cross(q, glm::quat(glm::inverse(mInertiaTensor) 
    * (torque - 4.0f * glm::cross(euler, mInertiaTensor * euler))));
}


