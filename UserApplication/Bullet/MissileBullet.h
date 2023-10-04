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
#include "Uint32Vector2.h"

class MissileBullet {

public://��{�֐�
	MissileBullet();
	~MissileBullet();

	void Initialize();
	void Update(const Vector3& EndPos);
	void Draw(ViewProjection& viewProjection_);


public:

	//�e�����o��
	void MakeBullet(Vector3& pos);

	void MakeSelectMissileBullet(Vector3& pos, Vector3& left, Vector3& top, Vector3& right, uint32_t& MakeCount);

private:
	//�e�̏��X�V
	void WorldTransUpdate();
	//�����Ă��邩�ǂ���
	void CheckBulletAlive();
	//����ł�Ƃ��͉��[���Ɋi�[
	void SetNotAlivePosition();
	//1�t���[���O�̃|�W�V����
	//void OldPosUpdate();

public://Getter
	Vector3 GetEnemyBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(EnemyBulletWorldTrans[bulletCount].matWorld_); }
	bool GetExpandingBullet() { return isExpanding; }

public://Setter


private://const�֘A
	static const uint32_t AllBulletCount = 100;

private://�N���X�֘A
	std::unique_ptr<Model> model_;
	Input* input_ = nullptr;
	WorldTransform EnemyBulletWorldTrans[AllBulletCount];

	//�����蔻��
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;

private://�ʃN���X����l�����炤


private://�N���X�ϐ�
	bool isBulletAlive[AllBulletCount];
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;
	bool isStartTracking[AllBulletCount];
	bool isNearPlayer[AllBulletCount];

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 40;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 5;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t DieMaxParticle = 5;
	uint32_t makeBulletCount = 0;
	uint32_t BulletNotTrackingTime[AllBulletCount];

	//�C�[�W���O
	uint32_t BulletEasingTime[AllBulletCount];
	uint32_t BulletMaxEasingTime[AllBulletCount];
	uint32_t BulletLarpEasingTime[AllBulletCount];
	uint32_t BulletLarpMaxEasingTime[AllBulletCount];

	float EnemyBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];
	float EnemyBulletMaxRadius = 5.0f;
	float LarpT = 0.0f;
	float BulletSpeed[AllBulletCount];
	float BulletStartSpeed = 1.0f;
	float BulletEndSpeed = 3.0f;
	float BulletLerpSpeed[AllBulletCount];
	float BulletLerpStartSpeed = 0.04f;
	float BulletLrrpEndSpeed = 1.5f;
	float BulletStartLerpTime = 0.04f;

	Vector3 BulletVector[AllBulletCount];//�ł��o��������
	Vector3 BulletOldPos[AllBulletCount];
	Vector3 BulletVelocity[AllBulletCount];
};