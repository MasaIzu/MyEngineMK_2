#pragma once
class EasingData
{
public:

	/// <summary>
	/// タイマーを動かす更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// イージングを開始する関数
	/// </summary>
	/// <param name="animationFrame">何fでイージングを行うか</param>
	void Start(float animationFrame);

	//イージングが起こっているかどうか(進行割合が0より大きく1より小さい時)
	bool GetActive() { return isActive ; }

	float GetTimeRate()const { return timeRate; }

private:
	long long nowCount = 0;		//PC内の現在時刻
	long long startCount = 0;
	long long endCount = 0;
	float maxTime = 0;
	float endTimer = 0;
	float timeRate = 0;	//イージングの進行割合(0で0%、1で100%)
	float isActive = false;

};

