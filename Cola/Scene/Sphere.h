#pragma once
#include "../Basic/Transform.h"
#include "../Render/Mesh.h"

class Sphere
{
public:
	struct Attributes
	{
		float mRadius;
		float mTessLevel;
		glm::vec3 mAlbedo;
		glm::vec3 mEmissive;
		float mRoughness;
		float mMetallic;
	};
	Sphere();
	Transform mTransform;
	Attributes mAttributes;
	Mesh mMesh;
	size_t mId;
};