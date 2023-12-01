#pragma once
#include "ExplosionParticleSmoke.h"

class Explosion
{

public://基本関数
	Explosion( );
	~Explosion( );

	//初期化
	void Initialize();
	//更新
	void CSUpdate(ID3D12GraphicsCommandList* cmdList,const Vector4& Pos);
	//描画
	void Draw(const ViewProjection& view);

public:
	void MakeParticle();

private://メンバ関数

public://Setter

public://Getter

private://クラス関連
	std::unique_ptr<ExplosionParticleSmoke> Smoke;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数
	bool isExp = false;

	uint32_t MaxParticle = 10000;

};