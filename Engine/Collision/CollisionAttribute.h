#pragma once
//当たり判定属性

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b0;//地形
const unsigned short COLLISION_ATTR_OBJECT = 0b1;//オブジェクト
const unsigned short COLLISION_ATTR_ALLIES = 0b10;//味方
const unsigned short COLLISION_ATTR_ENEMYS = 0b11;//敵

const unsigned short COLLISION_ATTR_ATTACK = 0b100;//攻撃
const unsigned short COLLISION_ATTR_NOTATTACK = 0b101;//攻撃判定なし
const unsigned short COLLISION_ATTR_MELEEATTACK = 0b110;//無敵
const unsigned short COLLISION_ATTR_INVINCIBLE = 0b111;//無敵

const unsigned short COLLISION_ATTR_ENEMY_ATTACK = 0b1000;//敵の攻撃
const unsigned short COLLISION_ATTR_ENEMY_BULLET_ATTACK = 0b1001;//敵の弾の攻撃

const unsigned short COLLISION_ATTR_CAMERA = 0b1010;//カメラ
const unsigned short COLLISION_ATTR_FIRSTRAIL = 0b1011;//スロープ
const unsigned short COLLISION_ATTR_RAIL = 0b1100;//スロープ
const unsigned short COLLISION_ATTR_FINALRAIL = 0b1101;//スロープ

