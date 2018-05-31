#pragma once
#include "../Render/Mesh.h"
#include "../Render/Texture.h"
#include "../Basic/Transform.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

class Model
{
public:
	struct Node
	{
		std::string	name;
		Node*		parent;
		Node**		childrens;
		size_t		numChildrens;
		glm::mat4	localTransform;
		glm::mat4	globalTransform;
		~Node() { for (size_t i(0); i < numChildrens; ++i) delete childrens[i]; }
	};

	struct RenderObj
	{
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string	albedo;
		std::string normal;
		std::string metallic;
		std::string roughness;
		std::string emissive;
		std::string ao;
		Node* node;
	};

	struct Animation
	{
		struct Channel
		{
			std::map<double, glm::vec3> posKeys;
			std::map<double, glm::quat> rotKeys;
			std::map<double, glm::vec3> scaKeys;
		};
		std::string	name;
		double durationInTick;
		double ticksPerSecond;
		std::map<std::string, Channel> channel;
	};

	Model(const char* path);
	~Model();
	void UpdateAnim(size_t index, double time);
	double GetAnimDuration(size_t index);
	int GetAnimCount();
	const char* GetAnimName(size_t index);
	Transform mModelTrans;
	std::vector<Transform> mMeshTrans;
	std::vector<RenderObj> mRenderObjs;
	std::vector<Animation> mAnimations;
	std::vector<glm::mat4> mBoneFinalTrans;
	// OpenGL
	std::vector<std::shared_ptr<Mesh>> mMeshes;
	std::vector<std::shared_ptr<Texture>> mAlbedos;
	std::vector<std::shared_ptr<Texture>> mNormals;
	std::vector<std::shared_ptr<Texture>> mRoughness;
	std::vector<std::shared_ptr<Texture>> mMetallics;
	std::vector<std::shared_ptr<Texture>> mEmissive;
	size_t mId;
private:
	glm::mat4 GetAnimatedTrans(Animation::Channel& channel, double timeInTick);
	void UpdateGlobalTrans(size_t index, double timeInTick, Node* node, const glm::mat4& parentTrans);
	void UpdateGlobalTransNoAnim(Node* node, const glm::mat4& parentTrans);
	std::string GetOtherTypeTexture(std::string name, std::string type);

	Node* mRootNode;
	std::string mPath;
	std::map<std::string, size_t> mBoneMap;
	std::vector<glm::mat4> mBoneOffsetTrans;
};