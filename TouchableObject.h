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
	static TouchableObject* Create(std::unique_ptr<Model>& model, WorldTransform& worldTrans);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(std::unique_ptr<Model>& model, WorldTransform& worldTrans);
};

