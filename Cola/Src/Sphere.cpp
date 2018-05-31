#include "../Scene/Sphere.h"
#include "../Basic/Shape.h"

Sphere::Sphere() :
	mMesh(gCubeVer, 24)
{
	mAttributes.mRadius = 0.5f;
	mAttributes.mTessLevel = 16.0f;
	mAttributes.mAlbedo = glm::vec3(1.0f, 1.0f, 1.0f);
	mAttributes.mEmissive = glm::vec3(0.0f);
	mAttributes.mRoughness = 0.2f;
	mAttributes.mMetallic = 1.0f;
}
