#pragma once
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>

#include <d3d12.h>
#include <d3dx12.h>

class FbxAnimation {
public:

	FbxAnimation();
	~FbxAnimation();

	void Load(const std::string& failPath);

	aiAnimation* GetAnimation(int AnimationNumber);

public: // �萔
	// ���f���i�[���[�g�p�X
	static const std::string baseDirectory;
	// �e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const std::string defaultTextureFileName;

private:

	static const int32_t MaxAnimation = 20;

	const aiScene* mScene;

};
