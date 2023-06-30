#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "ViewProjection.h"


class PlayerBullet {

public://��{�֐�
	PlayerBullet();
	~PlayerBullet();

	void Initialize();
	void Update();
	void Draw(ViewProjection& viewProjection_);

private:
	//�v���[���[�̈ړ�
	void BulletMove();

	//�ł��o���������
	void MakePlayerBullet();

public://Getter
	Vector3 GetPlayerPos(const uint32_t& bulletCount)const { return MyMath::GetWorldTransform(playerBulletWorldTrans[bulletCount].matWorld_); }

public://Setter


private://const�֘A
	static const uint32_t AllBulletCount = 5;


private://�N���X�֘A
	std::unique_ptr<Model> model_;
	WorldTransform playerBulletWorldTrans[AllBulletCount];

private://�ʃN���X����l�����炤


private://�v���C���[�N���X�ϐ�
	bool isBulletAlive[AllBulletCount];

	float playerBulletSpeed = 1.0f;
	Vector3 playerBulletMoveMent;//�ړ���


};