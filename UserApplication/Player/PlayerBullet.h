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
	//�e�̃R���g���[��
	void BulletControl(const uint32_t& BulletNum, const Vector3& BulletVec);
	//�������ŋȂ����悤��
	void MakeExpandingStunBullet();
	//�����������o�[�W�����̃o���b�g�g�咆�X�V
	void UpdateWhileExpanding(const Vector3& MakeBulletPos, const Vector3& BulletVec);

private:
	//�v���[���[�̈ړ�
	void BulletUpdate();
	//�e�̏��X�V
	void WorldTransUpdate();
	//�^�C�}�[�X�V
	void BulletAliveTimerUpdate();
	//�����Ă��邩�ǂ���
	void CheckBulletAlive();

public://Getter
	Vector3 GetPlayerBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }
	bool GetExpandingBullet() { return isExpanding; }

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
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 120;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;

	float playerBulletSpeed = 10.0f;
	float BulletRadius[AllBulletCount];
	float playerBulletMaxRadius = 5.0f;

	Vector3 playerBulletMoveMent[AllBulletCount];//�ړ���
	Vector3 BulletVector[AllBulletCount];//�ł��o��������

};