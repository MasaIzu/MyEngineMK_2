#pragma once
#include "NormalBullet.h"
#include <array>
#include <stdint.h>
#include "WorldTransform.h"
#include <FbxModel.h>
#include <FBXObject3d.h>
#include "ExplosionBullet.h"

class NormalGun
{
public://基本関数
	NormalGun(const unsigned short Attribute);
	~NormalGun();

	//初期化
	void Initialize(const Vector3& Pos,Model* BulletModel);
	//更新
	void Update(const Vector3& Pos,const Vector3& rot);
	//描画
	void Draw(const ViewProjection& viewProjection_);
	//Fbx描画
	void FbxDraw(const ViewProjection& viewProjection_);

	//パーティクル
	void CSUpdate(ID3D12GraphicsCommandList* commandList);
	void ParticleDraw(const ViewProjection& viewProjection_);
public:
	//発射
	void ShotBullet(const Vector3& BulletVec);

private:
	//ポジションの更新
	void UpdatePosition();

	//タイムの更新
	void TimeUpdate();

public:
	//見てる方向のゲット
	WorldTarnsLook GetLook()const;

private://コンスト
	const static uint32_t BulletMaxCount = 2;


private://クラス関連
	//バレットの数
	std::array<std::unique_ptr<ExplosionBullet>,BulletMaxCount> explosionBullet;
	std::unique_ptr<Model> model_;
	WorldTransform GunTrans;

	std::unique_ptr<FBXModel> fbxModel_;
	std::unique_ptr<FBXObject3d> fbxObj3d_;
private:

	uint32_t CoolTime = 0;
	uint32_t MaxCoolTime = 15;

	float BulletSpeed = 4.5f;

};

