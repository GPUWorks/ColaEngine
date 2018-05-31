#include "../Scene/Model.h"

#pragma comment (lib, "assimp-vc140-mt.lib")
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <functional>

Model::Model(const char* path)
{
	std::cout << "Load model:\n" << path << "\n";
	Assimp::Importer importer;// ����ʱ�Ὣ���صĳ�����Դһ������
	const aiScene *aiscene = importer.ReadFile(
		path, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs
	);// �ȴ��°汾ͳһ����ϵͳ
	if (!aiscene) {
		std::cout << "Load model failed!" << std::endl;
		return;
	}
	// ����·��
	mPath = std::string(path);
	mPath = mPath.substr(0, mPath.find_last_of('/'));
	// ����洢����Ŀռ�
	mRenderObjs.resize(aiscene->mNumMeshes);
	// ����������ؽڵ㺯��������ڵ���Ϣ������
	std::function<void(aiNode*, Node**)> LoadNode = [&](aiNode* ainode, Node** node)
	{
		// �½��ڵ�
		*node = new Node;
		(*node)->name = ainode->mName.data;
		aiMatrix4x4& m = ainode->mTransformation;
		(*node)->localTransform = glm::mat4(
			m.a1, m.b1, m.c1, m.d1,
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4
		);
		// ��������
		for (unsigned int i(0); i < ainode->mNumMeshes; ++i) {
			aiMesh* aimesh = aiscene->mMeshes[ainode->mMeshes[i]];
			RenderObj& mesh = mRenderObjs[ainode->mMeshes[i]];
			mesh.node = *node;
			// ���ض���
			mesh.vertices.resize(aimesh->mNumVertices);
			for (unsigned int i(0); i < aimesh->mNumVertices; ++i) {
				Mesh::Vertex& vertex = mesh.vertices[i];
				vertex.pos.x = aimesh->mVertices[i].x;
				vertex.pos.y = aimesh->mVertices[i].y;
				vertex.pos.z = aimesh->mVertices[i].z;
				vertex.normal.x = aimesh->mNormals[i].x;
				vertex.normal.y = aimesh->mNormals[i].y;
				vertex.normal.z = aimesh->mNormals[i].z;
				vertex.tangent.x = aimesh->mTangents[i].x;
				vertex.tangent.y = aimesh->mTangents[i].y;
				vertex.tangent.z = aimesh->mTangents[i].z;
				vertex.bitangent.x = aimesh->mBitangents[i].x;
				vertex.bitangent.y = aimesh->mBitangents[i].y;
				vertex.bitangent.z = aimesh->mBitangents[i].z;
				vertex.uv.x = aimesh->mTextureCoords[0][i].x;
				vertex.uv.y = aimesh->mTextureCoords[0][i].y;
			}
			// ��������
			mesh.indices.resize(aimesh->mNumFaces * 3);
			for (unsigned int i(0); i < aimesh->mNumFaces; ++i) {
				mesh.indices[i * 3] = aimesh->mFaces[i].mIndices[0];
				mesh.indices[i * 3 + 1] = aimesh->mFaces[i].mIndices[1];
				mesh.indices[i * 3 + 2] = aimesh->mFaces[i].mIndices[2];
			}
			// ��������·��
			if (aimesh->mMaterialIndex >= 0) {
				aiString ainame;
				aiMaterial* aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
				aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &ainame);
				std::string name(ainame.data);
				name = name.substr(name.find_last_of("/\\") + 1);
				mesh.albedo = mPath + "/textures/" + name;
				// ��������
				mesh.roughness = GetOtherTypeTexture(name, "roughness");
				mesh.metallic = GetOtherTypeTexture(name, "metallic");
				mesh.normal = GetOtherTypeTexture(name, "normal");
				mesh.emissive = GetOtherTypeTexture(name, "emissive");
			}
			// ���ع�����Ƥ��Ϣ
			if (aimesh->HasBones()) {
				for (unsigned int i(0); i < aimesh->mNumBones; ++i) {
					aiBone* aibone = aimesh->mBones[i];
					// �����Ƿ��¼�˸ù���
					auto it = mBoneMap.find(aibone->mName.data);
					size_t boneID;
					if (it == mBoneMap.end()) {
						aiMatrix4x4 m = aibone->mOffsetMatrix;
						glm::mat4 offset(
							m.a1, m.b1, m.c1, m.d1,
							m.a2, m.b2, m.c2, m.d2,
							m.a3, m.b3, m.c3, m.d3,
							m.a4, m.b4, m.c4, m.d4
						);
						boneID = mBoneOffsetTrans.size();
						mBoneMap[aibone->mName.data] = boneID;
						mBoneOffsetTrans.emplace_back(offset);
					}
					else {
						boneID = it->second;
					}
					// ��������Ϣ���붥��
					for (unsigned int i(0); i < aibone->mNumWeights; ++i) {
						auto& v = mesh.vertices[aibone->mWeights[i].mVertexId];
						for (unsigned int j(0); j < 4; ++j) {
							if (v.weights[j] == 0.0f) {
								v.weights[j] = aibone->mWeights[i].mWeight;
								v.boneIDs[j] = static_cast<int>(boneID);
								break;
							}
						}
					}
				}
			}
		}
		// �½��ӽڵ�
		(*node)->numChildrens = ainode->mNumChildren;
		(*node)->childrens = new Node*[(*node)->numChildrens];
		for (unsigned int i(0); i < ainode->mNumChildren; ++i) {
			// ���ø��ڵ�
			(*node)->childrens[i] = new Node;
			(*node)->childrens[i]->parent = *node;
			// ���µ���
			LoadNode(ainode->mChildren[i], &(*node)->childrens[i]);
		}
	};
	// ����������
	LoadNode(aiscene->mRootNode, &mRootNode);
	// Ϊ������Ƥ�Ĺ�������ռ�
	mBoneFinalTrans.resize(mBoneOffsetTrans.size());
	// ���ض���
	mAnimations.resize(aiscene->mNumAnimations);
	for (unsigned int i(0); i < aiscene->mNumAnimations; ++i) {
		Animation& anim = mAnimations[i];
		aiAnimation* aianim = aiscene->mAnimations[i];
		// Copy animations
		anim.name = aianim->mName.data;
		anim.durationInTick = aianim->mDuration;
		anim.ticksPerSecond = aianim->mTicksPerSecond;
		for (unsigned int i(0); i < aianim->mNumChannels; ++i) {
			aiNodeAnim* aichannel = aianim->mChannels[i];
			Animation::Channel channel;
			// Copy channels
			for (unsigned int i(0); i < aichannel->mNumPositionKeys; ++i) {
				// Copy positions
				aiVectorKey& posKey = aichannel->mPositionKeys[i];
				channel.posKeys[posKey.mTime] = glm::vec3(posKey.mValue.x, posKey.mValue.y, posKey.mValue.z);
			}
			for (unsigned int i(0); i < aichannel->mNumRotationKeys; ++i) {
				// Copy rotations
				aiQuatKey& rotKey = aichannel->mRotationKeys[i];
				// glm�����Ԫ�����캯��w�ڵ�һ��...
				channel.rotKeys[rotKey.mTime] = glm::quat(rotKey.mValue.w, rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z);
			}
			for (unsigned int i(0); i < aichannel->mNumScalingKeys; ++i) {
				// Copy scalings
				aiVectorKey& scaKey = aichannel->mScalingKeys[i];
				channel.scaKeys[scaKey.mTime] = glm::vec3(scaKey.mValue.x, scaKey.mValue.y, scaKey.mValue.z);
			}
			// Add channel to map
			anim.channel[aichannel->mNodeName.data] = channel;
		}
	}
	// ����

	UpdateAnim(999, 0.0);
	// ��¼�Ѽ��ص�����
	std::map<std::string, std::shared_ptr<Texture>> loadedTextures;
	// ��������OpenGL
	for (auto& renderObj : mRenderObjs) {
		// ����mesh��shared_ptr
		std::shared_ptr<Mesh> mesh(new Mesh(
			renderObj.vertices.data(), static_cast<unsigned int>(renderObj.vertices.size()),
			renderObj.indices.data(), static_cast<unsigned int>(renderObj.indices.size())
		));
		mMeshes.emplace_back(mesh);

		// ����
		// ��ѯ�����Ƿ��Ѽ��ع�
		auto it = loadedTextures.find(renderObj.albedo);
		if (it == loadedTextures.end()) {// û�������
			std::shared_ptr<Texture> texture(new Texture(renderObj.albedo.c_str()));
			loadedTextures[renderObj.albedo] = texture;
			mAlbedos.emplace_back(texture);
		}
		else {// �Ѽ�������
			mAlbedos.emplace_back(it->second);
		}
		// ������ͼ
		auto it1 = loadedTextures.find(renderObj.normal);
		if (it1 == loadedTextures.end()) {// û�������
			std::shared_ptr<Texture> texture(new Texture(renderObj.normal.c_str()));
			loadedTextures[renderObj.normal] = texture;
			mNormals.emplace_back(texture);
		}
		else {// �Ѽ�������
			mNormals.emplace_back(it1->second);
		}
		// �ֲڶ���ͼ
		auto it2 = loadedTextures.find(renderObj.roughness);
		if (it2 == loadedTextures.end()) {// û�������
			std::shared_ptr<Texture> texture(new Texture(renderObj.roughness.c_str()));
			loadedTextures[renderObj.roughness] = texture;
			mRoughness.emplace_back(texture);
		}
		else {// �Ѽ�������
			mRoughness.emplace_back(it2->second);
		}
		// ��������ͼ
		auto it3 = loadedTextures.find(renderObj.metallic);
		if (it3 == loadedTextures.end()) {// û�������
			std::shared_ptr<Texture> texture(new Texture(renderObj.metallic.c_str()));
			loadedTextures[renderObj.metallic] = texture;
			mMetallics.emplace_back(texture);
		}
		else {// �Ѽ�������
			mMetallics.emplace_back(it3->second);
		}
		// �Է�����ͼ
		auto it4 = loadedTextures.find(renderObj.emissive);
		if (it4 == loadedTextures.end()) {// û�������
			std::shared_ptr<Texture> texture(new Texture(renderObj.emissive.c_str()));
			loadedTextures[renderObj.emissive] = texture;
			mEmissive.emplace_back(texture);
		}
		else {// �Ѽ�������
			mEmissive.emplace_back(it4->second);
		}
		mMeshTrans.emplace_back(renderObj.node->globalTransform);
	}
}

Model::~Model()
{
	delete mRootNode;
}

void Model::UpdateAnim(size_t index, double time)
{
	if (index < mAnimations.size()) {
		double timeInTick = time * mAnimations[index].ticksPerSecond;
		UpdateGlobalTrans(index, timeInTick, mRootNode, glm::mat4(1.0f));
	}
	else {
		UpdateGlobalTransNoAnim(mRootNode, glm::mat4(1.0f));
	}
	for (size_t i(0); i < mMeshes.size(); ++i) {
		mMeshTrans[i].SetMatrix(mRenderObjs[i].node->globalTransform);
	}
}

double Model::GetAnimDuration(size_t index)
{
	if (index >= mAnimations.size()) return 0.0;
	return mAnimations[index].durationInTick / mAnimations[index].ticksPerSecond;
}

int Model::GetAnimCount()
{
	return static_cast<int>(mAnimations.size());
}

const char* Model::GetAnimName(size_t index)
{
	if (index >= mAnimations.size()) return nullptr;
	return mAnimations[index].name.c_str();
}

glm::mat4 Model::GetAnimatedTrans(Animation::Channel& channel, double timeInTick)
{
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 sca;
	// position
	auto it0 = channel.posKeys.lower_bound(timeInTick);
	if (it0 == channel.posKeys.begin()) pos = it0->second;
	else if (it0 == channel.posKeys.end()) pos = (--it0)->second;
	else {
		glm::vec3 p1 = it0->second;
		double t1 = it0->first;
		--it0;
		glm::vec3 p0 = it0->second;
		double t0 = it0->first;
		double k = (timeInTick - t0) / (t1 - t0);
		pos = glm::mix(p0, p1, k);
	}
	// rotation
	auto it1 = channel.rotKeys.lower_bound(timeInTick);
	if (it1 == channel.rotKeys.begin()) rot = it1->second;
	else if (it1 == channel.rotKeys.end()) rot = (--it1)->second;
	else {
		glm::quat r1 = it1->second;
		double t1 = it1->first;
		--it1;
		glm::quat r0 = it1->second;
		double t0 = it1->first;
		double k = (timeInTick - t0) / (t1 - t0);
		// ע��ʹ����Ԫ�������ֵ
		rot = glm::slerp(r0, r1, static_cast<float>(k));
	}
	// scaling
	auto it2 = channel.scaKeys.lower_bound(timeInTick);
	if (it2 == channel.scaKeys.begin()) sca = it2->second;
	else if (it2 == channel.scaKeys.end()) sca = (--it2)->second;
	else {
		glm::vec3 s1 = it2->second;
		double t1 = it2->first;
		--it2;
		glm::vec3 s0 = it2->second;
		double t0 = it2->first;
		double k = (timeInTick - t0) / (t1 - t0);
		sca = glm::mix(s0, s1, k);
	}
	// transform
	glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), pos) *
		glm::mat4_cast(rot) *
		glm::scale(glm::mat4(1.0f), sca);
	return transform;
}

void Model::UpdateGlobalTrans(size_t index, double timeInTick, Node* node, const glm::mat4& parentTrans)
{
	glm::mat4 thisTrans;
	auto channelIter = mAnimations[index].channel.find(node->name);
	// ����ýڵ�û�ж���
	if (channelIter == mAnimations[index].channel.end()) {
		thisTrans = parentTrans * node->localTransform;
	}
	// ��֮���ж����Ļ�
	else {
		thisTrans = parentTrans * GetAnimatedTrans(channelIter->second, timeInTick);
	}
	// ����
	node->globalTransform = thisTrans;
	// ����а󶨵Ĺ����������������ձ任
	auto boneIter = mBoneMap.find(node->name);
	if (boneIter != mBoneMap.end()) {
		mBoneFinalTrans[boneIter->second] = thisTrans * mBoneOffsetTrans[boneIter->second];
	}
	// ���µ���
	for (size_t i(0); i < node->numChildrens; ++i) {
		UpdateGlobalTrans(index, timeInTick, node->childrens[i], thisTrans);
	}
}

void Model::UpdateGlobalTransNoAnim(Node* node, const glm::mat4& parentTrans)
{
	glm::mat4 thisTrans = parentTrans * node->localTransform;
	node->globalTransform = thisTrans;
	// ���µ���
	for (size_t i(0); i < node->numChildrens; ++i) {
		UpdateGlobalTransNoAnim(node->childrens[i], thisTrans);
	}
}

std::string Model::GetOtherTypeTexture(std::string name, std::string type)
{
	std::string other(name);
	other = other.substr(0, other.find_last_of('_') + 1) + type + other.substr(other.find_last_of('.'));
	return mPath + "/textures/" + other;
}
