#include "SplinePosition.h"

SplinePosition::SplinePosition()
{
}

SplinePosition::SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end)
{
	std::vector<Vector3> points{ Start, Start, p1, p2, end, end };
	this->points = points;
}

SplinePosition::SplinePosition(const std::vector<Vector3>& points)
{
	this->points = points;
}

SplinePosition::~SplinePosition() {}

void SplinePosition::Update(float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);


}

void SplinePosition::Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end, float& time)
{
	std::vector<Vector3> points{ Start, Start, p1, p2, end, end };
	this->points = points;

	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
			isResetNearSpline = false;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);
}

void SplinePosition::Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& p3, Vector3& end, float& time)
{
	std::vector<Vector3> points{ Start, Start, p1, p2, p3, end, end };
	this->points = points;

	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);
}

void SplinePosition::Update(const std::vector<Vector3>& points, float& time)
{
	timeRate_ += time;
	if (timeRate_ >= MaxTime) {
		if (startIndex < points.size() - 3) {
			startIndex += 1;
			timeRate_ -= MaxTime;
		}
		else {
			timeRate_ = MaxTime;
			isFinishSpline = true;
		}
	}
	NowPos = SplinePositionUpdate(points, startIndex, timeRate_);
}

void SplinePosition::Reset()
{
	startIndex = 1;
	timeRate_ = 0.0f;
	isFinishSpline = false;
	isResetNearSpline = false;
}

void SplinePosition::Reset(const size_t& ResetIndex, float& time)
{
	startIndex = ResetIndex;
	timeRate_ = time;
	isFinishSpline = false;
}

void SplinePosition::ResetNearSpline(Vector3& Pos)
{
	if (isResetNearSpline == false) {
		for (uint32_t i = 1; i < points.size() - 1; i++) {
			Vector3 pos = Pos - points[i];
			Vector3 nextPoint = points[i + 1] - points[i];
			if (isFinishSpline == true) {
				if (0.0f < pos.length() && pos.length() < nextPoint.length()) {
					startIndex = i;
					timeRate_ = pos.length() / nextPoint.length();
					isFinishSpline = false;
					isResetNearSpline = true;
					break;
				}
			}
			else {
				if (0.0f < pos.length() && pos.length() < nextPoint.length()) {
					startIndex = i;
					timeRate_ = pos.length() / nextPoint.length();
					isResetNearSpline = true;
					break;
				}
			}
		}
	}
}

void SplinePosition::ResetNearSplineReset()
{
	isResetNearSpline = false;
}

Vector3 SplinePosition::SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t)
{
	//•âŠÔ‚·‚×‚«“_‚Ì”
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}

void SplinePosition::SetNotSplineVector(const std::vector<Vector3>& points)
{
	std::vector<Vector3> setPoints;
	uint32_t vecCount = 0;
	for (Vector3 ground : points) {
		setPoints.push_back(ground);
		if (vecCount == 0) {
			setPoints.push_back(ground);
		}
		else if (setPoints.size() - 1 == points.size()) {
			setPoints.push_back(ground);
		}
		vecCount++;
	}
	this->points = setPoints;
}
