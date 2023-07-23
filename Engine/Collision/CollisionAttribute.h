#pragma once

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b0;//地形
const unsigned short COLLISION_ATTR_OBJECT = 0b1;//オブジェクト
const unsigned short COLLISION_ATTR_ALLIES = 0b10;//味方
const unsigned short COLLISION_ATTR_ENEMYS = 0b11;//敵

const unsigned short COLLISION_ATTR_ATTACK = 0b100;//攻撃
const unsigned short COLLISION_ATTR_NOTATTACK = 0b101;//攻撃判定なし
const unsigned short COLLISION_ATTR_PLAYERINVINCIBLE = 0b110;//無敵
const unsigned short COLLISION_ATTR_ENEMYINVINCIBLE = 0b111;//無敵

const unsigned short COLLISION_ATTR_ENEMYATTACK = 0b1000;//敵の攻撃
const unsigned short COLLISION_ATTR_ENEMYBULLETATTACK = 0b1001;//敵の弾の攻撃

const unsigned short COLLISION_ATTR_CAMERA = 0b1010;//カメラ
const unsigned short COLLISION_ATTR_SROP = 0b1011;//スロープ