#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "TouchableObject.h"
#include <MeshCollider.h>

class Ground {

public:
	Ground(Model* model);
	void Initialze();

	void Update();

	void Draw(const ViewProjection& view);

private:

	Model* groundModel = nullptr;
	WorldTransform groundTrans;
	std::unique_ptr<TouchableObject> touchableObject;
	std::unique_ptr<MeshCollider> meshCollider;
};