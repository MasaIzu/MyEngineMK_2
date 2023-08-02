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
#include "ParticleManager.h"
#include "Uint32Vector2.h"

class PlayerBullet {

public://��{�֐�
	PlayerBullet();
	~PlayerBullet();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

public:
	//�ł��o���������
	uint32_t MakePlayerBullet(const Vector3& MakeBulletPos,const Vector3& BulletVec,const float& Distance);
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
	//����ł�Ƃ��͉��[���Ɋi�[
	void SetNotAlivePosition();
	//�p�[�e�B�N�����o��
	void MakeParticle(Vector3& pos, Vector3& BulletVelocity,const float& BulletSpeed);
	void MakeParticle(Vector3& pos, Vector3& BulletVelocity, const float& BulletSpeed, const float& DownScale);
	//1�t���[���O�̃|�W�V����
	void OldPosUpdate();

public://Getter
	Vector3 GetPlayerBulletPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }
	bool GetExpandingBullet() { return isExpanding; }

public://Setter
	

private://const�֘A
	static const uint32_t AllBulletCount = 30;

private://�N���X�֘A
	std::unique_ptr<Model> model_;
	WorldTransform playerBulletWorldTrans[AllBulletCount];

	//�����蔻��
	BaseCollider* BulletCollider[AllBulletCount];
	CollisionManager* collisionManager = nullptr;
	std::unique_ptr<ParticleManager> ParticleMan;
private://�ʃN���X����l�����炤
	

private://�v���C���[�N���X�ϐ�
	bool isBulletAlive[AllBulletCount];
	bool isExpanding = false;
	bool isMovingExpandingBullet = false;
	bool isBulletDownSpeed[AllBulletCount];

	int x = 0;

	uint32_t BulletLifeTime[AllBulletCount];
	uint32_t BulletSpeedDownTime[AllBulletCount];
	uint32_t BulletNum_ = 0;
	uint32_t MaxBulletLifeTime = 30;
	uint32_t BulletCoolTime = 0;
	uint32_t MaxBulletCoolTime = 7;
	uint32_t ParticleFile = 60;
	uint32_t MackPaticleMax = 1;
	uint32_t AttackMaxParticle = 10;
	uint32_t MakeBulletMaxParticle = 8;
	uint32_t DieMaxParticle = 30;
	uint32_t DieMaxParticleLife = 40;

	float playerBulletSpeed[AllBulletCount];
	float BulletRadius[AllBulletCount];
	float playerBulletMaxRadius = 5.0f;
	float PlayerParticleSpeed = 0.004f;
	float PlayerBulletMakeParticleSpeed = 0.2f;
	float PlayerParticleDieSpeed = 0.2f;
	float flame[AllBulletCount];
	float G = 3.0f;//�d��

	Vector3 playerBulletMoveMent[AllBulletCount];//�ړ���
	Vector3 BulletVector[AllBulletCount];//�ł��o��������
	Vector3 BulletOldPos[AllBulletCount];//1�t���[���O�̃|�W�V����
};