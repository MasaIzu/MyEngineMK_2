#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"
#include<random>

class TouchableObject;

struct ground
{
	WorldTransform pos;

};

struct block
{
	WorldTransform pos;//world���W

};
//add

class Ground {

public:

	Ground();
	~Ground();

	/// <summary>
	/// ������
	/// <summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update();

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void CreateGround();

	void CreateBlock();

	void CreateBlockV1(Vector3 pos);
	void CreateBlockV2(Vector3 pos);
	void CreateBlockV3(Vector3 pos);
	float Random(float num1, float num2);

	block newblock;

	static const int MaxCount = 5;
	//�n�ʍ쐬
	std::unique_ptr<Model>groundModel_ = nullptr;
	std::vector<ground> groundObjects;
	//�ǂ̃u���b�N�P
	std::unique_ptr<Model>blockModel_ = nullptr;
	std::vector<block>blockObjects;
	//�ǂ̃u���b�N�Q
	std::unique_ptr<Model>blockModelV2_ = nullptr;
	
	//�ǂ̃u���b�N�R
	std::unique_ptr<Model>blockModelV3_ = nullptr;
	
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	//std::unique_ptr<Model> model_ = nullptr;

	/*std::unique_ptr<Model> groundModel = nullptr;
	TouchableObject* objGround = nullptr;*/

	//�X�e�[�W�T�C�Y�̕ύX
	CONST FLOAT XtZ = 40;
	//�����̕ύX
	CONST FLOAT Y = -1.25f;


	

	

	//std::unique_ptr<Model> modeltable[groundMaxCount];

};