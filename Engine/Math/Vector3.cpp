#include "Vector3.h"

#include<cmath>  //sprt

Vector3::Vector3() :x(0), y(0), z(0)
{
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::length()const {
	return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

Vector3 Vector3::norm()const
{
	Vector3 nor = *this;

	float len = length();
	if (len != 0) {
		return nor /= len;
	}
	
	return nor;
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.x + x * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

//ê¸å`ï‚ä‘
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t) {
	
	return start * (1.0f - t) + end * t;
}

Vector3 Vector3::operator+()const {
	return *this;
}
Vector3 Vector3::operator-()const {
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//Vector3 ÉNÉâÉXÇ…ëÆÇ≥Ç»Ç¢ä÷êîåQ


const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	temp += v2;
	return temp;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	temp -= v2;
	return temp;
}

const Vector3 operator*(const Vector3& v, float s){
	Vector3 temp = v;
	temp.x = temp.x * s;
	temp.y = temp.y * s;
	temp.z = temp.z * s;

	return temp;
}

const Vector3 operator*(float s, const Vector3& v)
{
	Vector3 temp = v;
	temp.x = temp.x * s;
	temp.y = temp.y * s;
	temp.z = temp.z * s;

	return temp;
}

const Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp = v1;
	temp *= v2;
	return temp;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp = v;
	temp.x = temp.x / s;
	temp.y = temp.y / s;
	temp.z = temp.z / s;

	return temp;
}