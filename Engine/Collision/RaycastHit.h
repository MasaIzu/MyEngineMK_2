#pragma once

#include "BaseCollider.h"
#include<DirectXMath.h>
#include"Vector4.h"

struct RaycastHit {

	BaseCollider* collider = nullptr;

	Vector4 inter;

	float distance = 0.0f;

};