#pragma once
#pragma once
#include "Model.h"
#include "CollisionAttribute.h"

class TouchableObject
{
public:
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model, WorldTransform& worldTrans, unsigned short attribute);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model, WorldTransform& worldTrans, unsigned short attribute);
};

