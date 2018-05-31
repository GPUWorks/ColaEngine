#include "../Scene/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspect, float fov) :
	mAspect(aspect),
	mFov(glm::vec3(fov, 1.0f, 120.0f)),
	mVisualRange(glm::vec2(0.1f, 1000.0f))
{
	mFov.x = glm::clamp(mFov.x, mFov.y, mFov.z);
}

void Camera::SetAspect(float aspect)
{
	mAspect = aspect;
}

void Camera::SetFov(float fov)
{
	mFov.x = glm::clamp(fov, mFov.y, mFov.z);
}

glm::vec3 Camera::GetPos()
{
	return mTransform.GetPosition();
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(
		mTransform.GetPosition(),
		mTransform.GetPosition() - mTransform.GetLocalAxisZ(),
		mTransform.GetLocalAxisY()
	);
}

glm::mat4 Camera::GetProj()
{
	return glm::perspective(
		glm::radians(mFov.x),
		mAspect,
		mVisualRange.x,
		mVisualRange.y
	);
}
