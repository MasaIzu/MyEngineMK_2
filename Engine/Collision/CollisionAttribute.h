#pragma once

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b0;//�n�`
const unsigned short COLLISION_ATTR_OBJECT = 0b1;//�I�u�W�F�N�g
const unsigned short COLLISION_ATTR_ALLIES = 0b10;//����
const unsigned short COLLISION_ATTR_ENEMYS = 0b11;//�G

const unsigned short COLLISION_ATTR_ATTACK = 0b100;//�U��
const unsigned short COLLISION_ATTR_NOTATTACK = 0b101;//�U������Ȃ�
const unsigned short COLLISION_ATTR_PLAYERINVINCIBLE = 0b110;//���G
const unsigned short COLLISION_ATTR_ENEMYINVINCIBLE = 0b111;//���G

const unsigned short COLLISION_ATTR_ENEMYATTACK = 0b1000;//�G�̍U��
const unsigned short COLLISION_ATTR_ENEMYBULLETATTACK = 0b1001;//�G�̒e�̍U��

const unsigned short COLLISION_ATTR_CAMERA = 0b1010;//�J����
const unsigned short COLLISION_ATTR_FIRSTRAIL = 0b1011;//�X���[�v
const unsigned short COLLISION_ATTR_RAIL = 0b1100;//�X���[�v
const unsigned short COLLISION_ATTR_FINALRAIL = 0b1101;//�X���[�v