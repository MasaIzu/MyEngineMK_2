#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <Sprite.h>

class PlayerOperationUI
{
public://基本関数
	PlayerOperationUI( );
	~PlayerOperationUI( );

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

private://メンバ関数

public://Setter

public://Getter

private://クラス関連
	std::unique_ptr<Sprite> AttackUISprite;
	std::unique_ptr<Sprite> AttackMissileUISprite;
	std::unique_ptr<Sprite> BoostUISprite;
	std::unique_ptr<Sprite> OrUISprite;
	std::unique_ptr<Sprite> MouseLeftSprite;
	std::unique_ptr<Sprite> MouseRightSprite;
	std::unique_ptr<Sprite> EKeySprite;
	std::unique_ptr<Sprite> ShiftKeySprite;
	std::unique_ptr<Sprite> UpBoostSprite;
	std::unique_ptr<Sprite> SpaceSprite;

	//ボタン
	std::unique_ptr<Sprite> XBotanSprite;
	std::unique_ptr<Sprite> ABotanSprite;
	std::unique_ptr<Sprite> L1Sprite;
	std::unique_ptr<Sprite> L2Sprite;
	std::unique_ptr<Sprite> R2Sprite;

private://イーナムクラス

private://別クラスから値をもらう

private://クラス変数

	Vector2 AttackUIPos = { 1100.0f,50.0f };
	Vector2 MissileUIPos = { 1100.0f,90.0f };
	Vector2 BoostUIPos = { 1100.0f,130.0f };
	Vector2 MousLeftUIPos = { 1180.0f,50.0f };
	Vector2 MousRightUIPos = { 1235.0f,50.0f };
	Vector2 OrUIPos = { 1206.0f,53.0f };
	Vector2 EkeyPos = { 1178.0f,90.0f };
	Vector2 ShiftkeyPos = { 1189.0f,130.0f };
	Vector2 UpBoostUIPos = { 1100.0f,170.0f };
	Vector2 SpaceUIPos = { 1188.0f,170.0f };
	Vector2 XBotanPos = { 1178.0f,130.0f };
	Vector2 ABotanPos = { 1178.0f,170.0f };

	Vector2 UISize = { 115.0f,18.0f };
	Vector2 MousUISize = { 29.0f,29.0f };
	Vector2 OrUISize = { 20.0f,20.0f };
	Vector2 EkeySize = { 25.0f,25.0f };
	Vector2 ShiftkeySize = { 49.0f,25.0f };
	Vector2 SpacekeySize = { 49.0f,25.0f };

	Vector4 WhiteColor = { 1.0f,1.0f,1.0f,1.0f };
};