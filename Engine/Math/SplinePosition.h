#pragma once
#include <Vector3.h>
#include <memory>
#include <vector>

class SplinePosition {

public://基本関数
	/// <summary>
	/// 後で決める方
	/// </summary>
	SplinePosition();

	/// <summary>
	/// Vector3を4つ入れるバージョン(vector型で渡す方もある)
	/// </summary>
	/// <param name="Start">スタート地点</param>
	/// <param name="p1">制御点その1</param>
	/// <param name="p2">制御点その2</param>
	/// <param name="end">ゴール地点</param>
	SplinePosition(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end);

	/// <summary>
	/// スタート地点,制御点,ゴール地点を好きに決められる方
	/// </summary>
	/// <param name="points">points{ start, start, p1, p2, p3, end, end };みたいにp3を追加したり</param>
	SplinePosition(const std::vector<Vector3>& points);
	~SplinePosition();

	/// <summary>
	/// スプライン曲線アップデート
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(float& time);

	/// <summary>
	/// スプライン曲線アップデート4つ打ち込むVer
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& end, float& time);

	/// <summary>
	/// スプライン曲線アップデート5つ打ち込むVer(5個までなら手打ちでいける)
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(Vector3& Start, Vector3& p1, Vector3& p2, Vector3& p3, Vector3& end, float& time);

	/// <summary>
	/// スプライン曲線アップデートpointsもアプデVer
	/// </summary>
	/// <param name="time">デフォMaxTime=1のうちのどこか</param>
	void Update(const std::vector<Vector3>& points, float& time);

	/// <summary>
	/// スタートに戻すVer
	/// </summary>
	void Reset();

	/// <summary>
	/// 細かくリセットを決められるVer
	/// </summary>
	/// <param name="ResetIndex">Indexはスタートに戻したい場合は1にする(0は無理)</param>
	/// <param name="time">MaxTime=1でtimeが0.5なら半分から始まる</param>
	void Reset(const size_t& ResetIndex, float& time);

private://プライベート関数
	Vector3 SplinePositionUpdate(const std::vector<Vector3>& points, size_t& startIndex, float& t);

public://Setter
	void SetSplineMaxTime(const float& MaxTime) { this->MaxTime = MaxTime; }

public://Getter
	bool GetFinishSpline()const { return isFinishSpline; }

public://公開変数
	Vector3 NowPos;

private://クラス変数
	bool isFinishSpline = false;
	size_t startIndex = 1;
	float timeRate_ = 0.0f;
	float MaxTime = 1.0f;
	std::vector<Vector3> points{};

};