#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
	Transform();
	Transform(const glm::mat4& matrix);
	// get
	const glm::mat4& GetMatrix() const;
	const glm::vec3& GetPosition() const;
	const glm::vec3& GetScaling() const;
	const glm::quat& GetRotation() const;
	glm::vec3 GetRotationEuler() const;
	const glm::vec3& GetLocalAxisX() const;
	const glm::vec3& GetLocalAxisY() const;
	const glm::vec3& GetLocalAxisZ() const;
	// set
	void SetMatrix(const glm::mat4& matrix);
	void SetPosition(const glm::vec3& position);
	void SetScaling(const glm::vec3& scaling);
	void SetRotation(const glm::quat& rotation);
	void SetRotationEuler(const glm::vec3& rotation);

	void Move(const glm::vec3& position);
	void Scale(const glm::vec3& scaling);
	void Rotate(const glm::quat& rotation);
	void RotateEuler(const glm::vec3& rotation);

	void Mix(const Transform& t0, const Transform& t1, float k);
private:
	mutable bool mChanged;
	mutable glm::mat4 mMatrix;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScaling;

	glm::vec3 mLocalAxisX;
	glm::vec3 mLocalAxisY;
	glm::vec3 mLocalAxisZ;
};
