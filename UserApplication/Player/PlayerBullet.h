#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"
#include <BaseCollider.h>
#include <CollisionManager.h>

class PlayerBullet {

public://��{�֐�
	PlayerBullet();
	~PlayerBullet();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

public:
	//�ł��o���������
	uint32_t MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec);
	//�������ŋȂ����悤��
	void InputingBulletMove(const uint32_t& bulletNum, const Vector3& BulletVec);

private:
	//�v���[���[�̈ړ�
	void BulletMove();
	//�e�̏��X�V
	void WorldTransUpdate();
	//�^�C�}�[�X�V
	void BulletAliveTimerUpdate();
	//�����Ă��邩�ǂ���
	void CheckBulletAlive();

public://Getter
	Vector3 GetPlayerPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }

public://Setter
	

private://const�֘A
	static const uint32_t AllBulletCount = 50;

private://�N���X�֘A
	std::unique_ptr<Model> model_;
	WorldTransform playerBulletWorldTrans[AllBulletCount];

	//�����蔻��
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;

private://�ʃN���X����l�����炤
	

private://�v���C���[�N���X�ϐ�
	bool isBulletAlive[AllBulletCount];

	uint32_t BulletLifeTime[AllBulletCount];

	float playerBulletSpeed = 3.0f;
	float BulletRadius[AllBulletCount];

	Vector3 playerBulletMoveMent[AllBulletCount];//�ړ���
	Vector3 BulletVector[AllBulletCount];//�ł��o��������


};