#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "ParticleEditor.h"

/// <summary>
/// 地面
/// </summary>
class Torch
{
public:
	Torch(Model* model);
	//初期化
	void Initialze(const WorldTransform& trans);
	//更新
	void Update();
	//更新
	void CSUpdate(ID3D12GraphicsCommandList* cmdList,const bool& isNotBlack = true);
	//描画
	void Draw(const ViewProjection& view,const ViewProjection& LightViewProjection_);
	//描画
	void ShadowDraw(const ViewProjection& view);
	//描画
	void ParticleDraw(const ViewProjection& view);

private:

	std::unique_ptr<ParticleEditor> TorchParticle;
	Model* torchModel = nullptr;
	WorldTransform torchTrans;

};