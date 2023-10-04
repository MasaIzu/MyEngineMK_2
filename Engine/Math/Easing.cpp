#include "Easing.h"
#include"math.h"
//’Ç‰Á
double Easing::In(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * time * time);
}

double Easing::Out(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	return start + (move * (1 - (1 - time) * (1 - time)));
}

double Easing::InOut(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	if (time < 0.5)
	{
		return start + (move * (2 * time * time));
	}
	else
	{
		return start + move * (1 - (((-2 * time + 2) * (-2 * time + 2)) / 2));
	}
}

double Easing::In_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (c3 * time * time * time - c1 * time * time));
}

double Easing::Out_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c3 = (c1 + 1);
	return start + (move * (1 + c3 * ((time - 1) * (time - 1) * (time - 1)) + c1 * ((time - 1) * (time - 1))));
}

double Easing::InOut_Back(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double c1 = 1.70158;
	double c2 = c1 * 1.525;

	if (time < 0.5)
	{
		return start + move * ((((2 * time) * (2 * time)) * ((c2 + 1) * 2 * time - c2)) / 2);
	}
	else
	{
		return start + move * ((((2 * time - 2) * (2 * time - 2)) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2);
	}
}

double Easing::Out_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 1 / d1) {
		return start + move * (n1 * time * time);
	}
	else if (time < 2 / d1) {
		return start + move * (n1 * (time -= 1.5 / d1) * time + 0.75);
	}
	else if (time < 2.5 / d1) {
		return start + move * (n1 * (time -= 2.25 / d1) * time + 0.9375);
	}
	else {
		return start + move * (n1 * (time -= 2.625 / d1) * time + 0.984375);
	}
}

double Easing::In_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	time = 1 - time;
	if (time < 1 / d1) {
		return start + move * (1 - (n1 * time * time));
	}
	else if (time < 2 / d1) {
		return start + move * (1 - (n1 * (time -= 1.5 / d1) * time + 0.75));
	}
	else if (time < 2.5 / d1) {
		return start + move * (1 - (n1 * (time -= 2.25 / d1) * time + 0.9375));
	}
	else {
		return start + move * (1 - (n1 * (time -= 2.625 / d1) * time + 0.984375));
	}

}

double Easing::InOut_Bounce(double start, double end, double time, double max_time)
{
	time /= max_time;
	double move = end - start;
	double n1 = 7.5625;
	double d1 = 2.75;
	if (time < 0.5)
	{
		time = 1 - time * 2;
		if (time < 1 / d1) {
			return start + move * ((1 - (n1 * time * time)) / 2);
		}
		else if (time < 2 / d1) {
			return start + move * ((1 - (n1 * (time -= 1.5 / d1) * time + 0.75)) / 2);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((1 - (n1 * (time -= 2.25 / d1) * time + 0.9375)) / 2);
		}
		else {
			return start + move * ((1 - (n1 * (time -= 2.625 / d1) * time + 0.984375)) / 2);
		}
	}
	else
	{
		time = time * 2 - 1;
		if (time < 1 / d1) {
			return start + move * ((n1 * time * time) / 2 + 0.5);
		}
		else if (time < 2 / d1) {
			return start + move * ((n1 * (time -= 1.5 / d1) * time + 0.75) / 2 + 0.5);
		}
		else if (time < 2.5 / d1) {
			return start + move * ((n1 * (time -= 2.25 / d1) * time + 0.9375) / 2 + 0.5);
		}
		else {
			return start + move * ((n1 * (time -= 2.625 / d1) * time + 0.984375) / 2 + 0.5);
		}
	}
}


double Easing::easeOutCubic(double start, double end, double time, double max_time){
	time /= max_time;

	double x = 1 - pow(1 - time, 3);

	return start + (end - start) * x;
}

float Easing::easeOutCubic(float& start, float& end, uint32_t& time, uint32_t& max_time)
{
	float Time = static_cast<float>(time) / static_cast<float>(max_time);
	float x = 1 - powf(1 - Time, 3);
	return start + (end - start) * x;
}

float Easing::EaseOutQuart(float& start, float& end, float& time, float& max_time)
{
	float Time = time / max_time;
	float x = 1 - powf(1 - Time, 4);
	return start + (end - start) * x;
}

float Easing::EaseOutQuint(float& start, float& end, float& time, float& max_time)
{
	float Time = time / max_time;
	float x = 1 - powf(1 - Time, 5);
	return start + (end - start) * x;
}

Vector3 Easing::EaseOutCubicVec3(Vector3& start, Vector3& end, double& time, double& max_time)
{
	Vector3 easeOutCubicVec3;
	double x = easeOutCubic(static_cast <double>(start.x), static_cast <double>(end.x), time, max_time);
	double y = easeOutCubic(static_cast <double>(start.y), static_cast <double>(end.y), time, max_time);
	double z = easeOutCubic(static_cast <double>(start.z), static_cast <double>(end.z), time, max_time);

	easeOutCubicVec3 = { static_cast <float>(x),static_cast <float>(y),static_cast <float>(z) };

	return easeOutCubicVec3;
}

Vector3 Easing::EaseOutQuartVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time)
{
	Vector3 easeOutQuartVec3;

	float timef = static_cast<float>(time);
	float max_timef = static_cast<float>(max_time);

	easeOutQuartVec3.x = EaseOutQuart(start.x, end.x, timef, max_timef);
	easeOutQuartVec3.y = EaseOutQuart(start.y, end.y, timef, max_timef);
	easeOutQuartVec3.z = EaseOutQuart(start.z, end.z, timef, max_timef);

	return easeOutQuartVec3;
}

Vector3 Easing::EaseOutQuintVec3(Vector3& start, Vector3& end, uint32_t& time, uint32_t& max_time)
{
	Vector3 easeOutQuintVec3;

	float timef = static_cast<float>(time);
	float max_timef = static_cast<float>(max_time);

	easeOutQuintVec3.x = EaseOutQuint(start.x, end.x, timef, max_timef);
	easeOutQuintVec3.y = EaseOutQuint(start.y, end.y, timef, max_timef);
	easeOutQuintVec3.z = EaseOutQuint(start.z, end.z, timef, max_timef);

	return easeOutQuintVec3;
}
