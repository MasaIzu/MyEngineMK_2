#pragma once
#include "Vector3.h"


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <cstdint>
MY_SUPPRESS_WARNINGS_END

//イージング
namespace Easing
{
	//動きと詳細はイージングチートシートを見て確認してくれ
	double In(double start, double end, double time, double max_time);
	double Out(double start, double end, double time, double max_time);
	double InOut(double start, double end, double time, double max_time);
	double In_Back(double start, double end, double time, double max_time);
	float In_Back(float& start,float& end,uint32_t& time,uint32_t& max_time);
	double Out_Back(double start, double end, double time, double max_time);
	double InOut_Back(double start, double end, double time, double max_time);
	double Out_Bounce(double start, double end, double time, double max_time);
	double In_Bounce(double start, double end, double time, double max_time);
	double InOut_Bounce(double start, double end, double time, double max_time);
	double easeOutCubic(double start, double end, double time, double max_time);
	float easeOutCubic(float& start, float& end, uint32_t& time, uint32_t& max_time);
	float EaseOutQuart(float& start, float& end, float& time, float& max_time);
	float EaseOutQuint(float& start, float& end, float& time, float& max_time);

	//Vector3にしただけ
	Vector3 EaseInBackVec3(Vector3& start,Vector3& end,uint32_t& time,uint32_t& max_time);
	Vector3 EaseOutCubicVec3(Vector3& start, Vector3& end, double& time, double& max_time);
	Vector3 EaseOutQuartVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time);
	Vector3 EaseOutQuintVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time);

};
