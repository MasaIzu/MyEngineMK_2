#pragma once
#pragma once
#include "Model.h"
#include "CollisionAttribute.h"

class TouchableObject
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(std::unique_ptr<Model>& model, WorldTransform& worldTrans);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(std::unique_ptr<Model>& model, WorldTransform& worldTrans);
};

