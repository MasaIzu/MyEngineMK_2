#pragma once

#include <string>
#include <vector>
#include "Vector3.h"
#include <Model.h>
#include "WorldTransform.h"
#include <map>
#include "TutorialEnemy.h"
#include "BulletShotEnemy.h"

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		Vector3 translation;
		// ��]�p
		Vector3 rotation;
		// �X�P�[�����O
		Vector3 scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

struct ModelData {
	Model* model;
	WorldTransform worldTrans;
};

// ���x���f�[�^�̐���
class LoadLevelEditor {

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string kDefaultBaseDirectory;
	// �t�@�C���g���q
	static const std::string kExtension;

public:// �����o�֐�

	LoadLevelEditor();
	~LoadLevelEditor();

	/// <summary>
	/// ���x���f�[�^�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	LevelData* LoadFile(const std::string& fileName);

	void Initialize(const std::string& fileName);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	std::list<TutorialEnemy*> GetEnemyList() { return tutorialEnemyList; }

private:
	std::unique_ptr<LevelData> levelData;
	std::map<std::string, Model*> models;
	std::vector<ModelData> LoadedObjects;
	std::vector<ModelData> NewLoadObjects;
	std::list<TutorialEnemy*> tutorialEnemyList;

	//�ŏ�����ǂݍ��ނ��
	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelFighter = nullptr;
	std::unique_ptr<Model> modelSphere;
};