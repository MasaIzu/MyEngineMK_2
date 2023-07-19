#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "TouchableObject.h"
#include <MeshCollider.h>

class Ground {

public:
	void Initialze();

	void Update();

	void Draw(const ViewProjection& view);

private:

	std::unique_ptr<Model> groundModel;
	WorldTransform groundTrans;
	std::unique_ptr<TouchableObject> touchableObject;
	std::unique_ptr<MeshCollider> meshCollider;
};