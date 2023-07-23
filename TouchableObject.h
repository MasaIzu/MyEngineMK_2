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
	static TouchableObject* Create(Model* model, WorldTransform& worldTrans, unsigned short attribute);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model, WorldTransform& worldTrans, unsigned short attribute);
};

