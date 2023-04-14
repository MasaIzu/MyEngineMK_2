#include "EasingData.h"
#include<time.h>
#include<cmath>

void EasingData::Update()
{

	if (!isActive) {
		//アクティブじゃないなら何もしない
		return;
	}

	//現在時刻を取得
	nowCount = clock();
	//取得されたイージング開始時間で終了時間が決まる
	endCount = nowCount - startCount;
	//タイマーの進み具合をfloat型に変換
	endTimer = static_cast<float>(endCount) / 1'000.0f;
	//(タイマーの進行度/最大時間)でイージングの進み具合を計算
	timeRate = endTimer / maxTime * 60.0f;
	//進行割合が1以上になったら(進行度100%)イージング終了
	if (timeRate >= 1.0f) {
		timeRate = 1.0f;
		isActive = false;
	}
}

void EasingData::Start(float animationFrame)
{
	startCount = clock();
	maxTime = animationFrame;
	isActive = true;
}
