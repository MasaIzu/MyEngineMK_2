#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"

class Ground {

public:
	void Initialze();

	void Update();

	void Draw(const ViewProjection& view);

private:

	std::unique_ptr<Model> groundModel;
	WorldTransform groundTrans;


};