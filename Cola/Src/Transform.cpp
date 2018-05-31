#include "../Basic/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Transform::Transform() :
	mChanged(true),
	mMatrix(1.0f),
	mPosition(0.0f),
	mRotation(),
	mScaling(1.0f),
	mLocalAxisX(1.0f, 0.0f, 0.0f),
	mLocalAxisY(0.0f, 1.0f, 0.0f),
	mLocalAxisZ(0.0f, 0.0f, 1.0f)
{
}

Transform::Transform(const glm::mat4& matrix)
{
	SetMatrix(matrix);
}

const glm::mat4& Transform::GetMatrix() const
{
	if (mChanged) {
		mMatrix =
			glm::translate(glm::mat4(1.0f), mPosition) *
			glm::mat4_cast(mRotation) *
			glm::scale(glm::mat4(1.0f), mScaling);
		mChanged = false;
	}
	return mMatrix;
}

const glm::vec3& Transform::GetPosition() const
{
	return mPosition;
}

const glm::vec3& Transform::GetScaling() const
{
	return mScaling;
}

const glm::quat& Transform::GetRotation() const
{
	return mRotation;
}

glm::vec3 Transform::GetRotationEuler() const
{
	return glm::eulerAngles(mRotation);
}

const glm::vec3& Transform::GetLocalAxisX() const
{
	return mLocalAxisX;
}

const glm::vec3& Transform::GetLocalAxisY() const
{
	return mLocalAxisY;
}

const glm::vec3& Transform::GetLocalAxisZ() const
{
	return mLocalAxisZ;
}

void Transform::SetMatrix(const glm::mat4& matrix)
{
	mChanged = false;
	mMatrix = matrix;
	// decompose mat4 to positon, rotation, scaling
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mMatrix, mScaling, mRotation, mPosition, skew, perspective);
	mRotation = glm::conjugate(mRotation);// 返回的四元数是共轭值，需做一次转换

	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

void Transform::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	mChanged = true;
}

void Transform::SetScaling(const glm::vec3& scaling)
{
	mScaling = scaling;
	mChanged = true;
}

void Transform::SetRotation(const glm::quat& rotation)
{
	mRotation = rotation;
	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
	mChanged = true;
}

void Transform::SetRotationEuler(const glm::vec3& rotation)
{
	mRotation = glm::quat(rotation);
	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
	mChanged = true;
}

void Transform::Move(const glm::vec3& position)
{
	mPosition += position;
	mChanged = true;
}

void Transform::Scale(const glm::vec3& scaling)
{
	mScaling += scaling;
	mChanged = true;
}

void Transform::Rotate(const glm::quat& rotation)
{
	mRotation = rotation * mRotation;
	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
	mChanged = true;
}

void Transform::RotateEuler(const glm::vec3& rotation)
{
	mRotation = glm::quat(rotation) * mRotation;
	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
	mChanged = true;
}

void Transform::Mix(const Transform& t0, const Transform& t1, float k)
{
	mPosition = glm::mix(t0.GetPosition(), t1.GetPosition(), k);
	mRotation = glm::slerp(t0.GetRotation(), t1.GetRotation(), k);
	mScaling = glm::mix(t0.GetScaling(), t1.GetScaling(), k);

	mLocalAxisX = mRotation * glm::vec3(1.0f, 0.0f, 0.0f);
	mLocalAxisY = mRotation * glm::vec3(0.0f, 1.0f, 0.0f);
	mLocalAxisZ = mRotation * glm::vec3(0.0f, 0.0f, 1.0f);
	mChanged = true;
}
