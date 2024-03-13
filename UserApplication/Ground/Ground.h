#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "TouchableObject.h"
#include <MeshCollider.h>

/// <summary>
/// 地面
/// </summary>
class Ground {


public:
	Ground(Model* model);
	//初期化
	void Initialze();
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& view,const ViewProjection& LightViewProjection_);
	//描画
	void ShadowDraw(const ViewProjection& view);

private:

	Model* groundModel = nullptr;
	WorldTransform groundTrans;
	std::unique_ptr<TouchableObject> touchableObject;
	std::unique_ptr<MeshCollider> meshCollider;
};