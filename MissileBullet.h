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
	static const uint32_t AllBulletCount = 30;

private://�N���X�֘A
	std::unique_ptr<Model> model_;
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

	float EnemyBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];
	float EnemyBulletMaxRadius = 5.0f;
	float EnemyParticleSpeed = 0.02f;
	float EnemyParticleDieSpeed = 0.2f;

	Vector3 EnemyBulletMoveMent[AllBulletCount];//�ړ���
	Vector3 BulletVector[AllBulletCount];//�ł��o��������
	Vector3 BulletOldPos[AllBulletCount];//1�t���[���O�̃|�W�V����
	Vector3 BulletTrackingStartPos[AllBulletCount];
};