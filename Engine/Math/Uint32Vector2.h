#pragma once
#include <cstdint>

class Uint32Vector2
{
public:
	uint32_t x;//x����
	uint32_t y;//y����

public:
	//�R���X�g���N�^
	Uint32Vector2();							//��x�N�g���Ƃ��Đ���
	Uint32Vector2(uint32_t x, uint32_t y);			//x����,y�������w�肵�Đ���


	//������Z�q�I�[�o�[���[�h
	Uint32Vector2& operator+=(const Uint32Vector2& v);
	Uint32Vector2& operator-=(const Uint32Vector2& v);
	Uint32Vector2& operator*=(uint32_t s);
	Uint32Vector2& operator/=(uint32_t s);

};

//2���ډ��Z�q�I�[�o�[���[�h
//���낢��Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɂ��Ă���

const Uint32Vector2 operator+(const Uint32Vector2& v1, const Uint32Vector2& v2);
const Uint32Vector2 operator-(const Uint32Vector2& v1, const Uint32Vector2& v2);
const Uint32Vector2 operator*(const Uint32Vector2& v, uint32_t s);
const Uint32Vector2 operator*(uint32_t s, const Uint32Vector2& v);
const Uint32Vector2 operator/(const Uint32Vector2& v, uint32_t s);