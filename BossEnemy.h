#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include <Input.h>
#include "SplinePosition.h"

class BossEnemy {

public://��{�֐�
	BossEnemy();
	~BossEnemy();

	void Initialize(const Vector3& Pos, ViewProjection* viewProjection);
	void Update();
	void Draw(ViewProjection& viewProjection_);


	//�p�[�e�B�N�����o���p
	void CSUpdate(ID3D12GraphicsCommandList* cmdList);
	void ParticleDraw(ViewProjection& viewProjection_);
	void CopyParticle();

	//���o�p����
	void StagingUpdate();

private://�֐�
	
	void WorldTransUpdate();//�ړ��̒l�X�V

public://Setter
	
	void SetStageMoveSpline(const std::vector<Vector3>& points) { MoveSpline->SetNotSplineVector(points); }

public://Getter

	bool GetHowReturnSpline(const uint32_t& HowIndex)const { return MoveSpline->GetHowReturnIndex(HowIndex); }
	bool GetFinishSpline()const { return MoveSpline->GetFinishSpline(); }

private://const�֘A
	static const uint32_t BossEnemyBodyCount = 20;

private://�N���X�֘A
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	WorldTransform BossWorldTrans[BossEnemyBodyCount];

	WorldTransform DebugWorldTrans;
	ViewProjection* viewProjection_ = nullptr;

	//�X�v���C��
	std::unique_ptr<SplinePosition> MoveSpline;//�r���̃X�v���C��

private://�C�[�i���N���X


private://�ʃN���X����l�����炤


private://EnemyBoss�N���X�ϐ�

	float EnemySplineUpdate = 0.015f;
	float MaxScale = 10.0f;

	Vector3 BonePos;

};