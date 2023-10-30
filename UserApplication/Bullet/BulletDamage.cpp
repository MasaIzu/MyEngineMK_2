#include "BulletDamage.h"

BulletDamage* BulletDamage::GetInstance()
{
	static BulletDamage instance;
	return &instance;
}
