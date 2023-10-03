#pragma once
#include "Vector3.h"
#include <cstdint>

//’Ç‰Á
namespace Easing
{
	double In(double start, double end, double time, double max_time);

	double Out(double start, double end, double time, double max_time);

	double InOut(double start, double end, double time, double max_time);

	double In_Back(double start, double end, double time, double max_time);

	double Out_Back(double start, double end, double time, double max_time);

	double InOut_Back(double start, double end, double time, double max_time);

	double Out_Bounce(double start, double end, double time, double max_time);

	double In_Bounce(double start, double end, double time, double max_time);

	double InOut_Bounce(double start, double end, double time, double max_time);

	double easeOutCubic(double start, double end, double time, double max_time);

	float EaseOutQuart(float& start, float& end, float& time, float& max_time);
	float EaseOutQuint(float& start, float& end, float& time, float& max_time);

	Vector3 EaseOutCubicVec3(Vector3& start, Vector3& end, double& time, double& max_time);
	Vector3 EaseOutQuartVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time);
	Vector3 EaseOutQuintVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time);

};
