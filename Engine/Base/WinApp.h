#pragma once

#include "Vector2.h"
#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN

#include<Windows.h>
#include <cstdint>
#include <string>

MY_SUPPRESS_WARNINGS_END

/// <summary>
/// ウィンドウ生成
/// </summary>
class WinApp
{
public:

	// ウィンドウサイズ
	static const uint32_t window_width; // 横幅
	static const uint32_t window_height; // 縦幅

public: // 静的メンバ関数

	// シングルトンインスタンスの取得
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数
	void MakeWindow(std::wstring title);
	bool ProcessMessage();
	void DeleteGameWindow();

	void Destroy();

	HINSTANCE GetHInstance() const { return w.hInstance; }
	Vector2 GetWindowSize() const;

public://GetterSetter

	HWND Gethwnd() { return hwnd; }

private://メンバ変数
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

	WNDCLASSEX w{};
	HWND hwnd;

private:
	static WinApp* WinApp_;

};

