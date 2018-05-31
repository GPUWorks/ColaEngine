#pragma once
#include "../Basic/Transform.h"

class Camera
{
public:
	Camera(float aspect, float fov);
	void SetAspect(float aspect);
	void SetFov(float fov);
	glm::vec3 GetPos();
	glm::mat4 GetView();
	glm::mat4 GetProj();
	Transform mTransform;
private:
	float mAspect;
	glm::vec3 mFov;
	glm::vec2 mVisualRange;
};