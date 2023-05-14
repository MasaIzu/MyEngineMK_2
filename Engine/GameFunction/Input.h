#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"
#include "Mouse.h"



// 入力
class Input
{

public: // メンバ関数

	static Input* GetInstance();
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// キーの押したかをチェック(長押し)
	bool PushKey(BYTE keyNumber);

	// キーのトリガーをチェック(押した瞬間)
	bool TriggerKey(BYTE keyNumber);

	// キーのトリガーをチェック(離した瞬間)
	bool ReleasedKey(BYTE keyNumber);

	// マウスボタンのトリガー入力
	bool MouseInputTrigger(int button);


	// マウスボタンの入力
	bool MouseInputing(int button);


	// マウスボタンの離した瞬間
	bool MouseOffTrigger(int button);

	// マウスの位置
	const Vector2 GetMousePos()const;

	// マウスの位置
	const Vector3 GetMouseWheelMove();

	void Destroy();

private:
	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

private:// 静的メンバ変数

	static Input* Input_;

private:
	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_ = nullptr;

	// キーボードのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_ = nullptr;

	// 全キーの状態
	BYTE key[256] = {};

	// 前回の全キーの状態
	BYTE keyPre[256] = {};

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	HWND hwnd_;

	//マウス
	std::unique_ptr<Mouse> mouse;
};
