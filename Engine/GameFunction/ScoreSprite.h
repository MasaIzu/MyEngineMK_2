#pragma once

#include "Sprite.h"
#include <memory>
#include "Vector2.h"
#include "Vector4.h"
#include <list>

class ScoreSprite
{
public:
    ScoreSprite();
    ~ScoreSprite();

    // 初期化の関数
    void Initialize();

    // 描画の関数
    void Draw(int num, Vector2 pos);

    void DrawSpeed(int num, Vector2 pos);

    void SetSize(Vector2 size);

private:
    // スプライト
    std::unique_ptr <Sprite> scoresprite[10];
    std::unique_ptr <Sprite> scoresprite2[10];

    // 初期化用変数
    Vector4 color = { 1, 1, 1, 1 };
    Vector2 anker = { 0, 0 };
    Vector2 size = { 50,100 };
    //スプライト一枚あたりのサイズ
    const Vector2 texsize = { 80,80 };
    Vector2 texBase = { 0,0 };

    // 初期化用変数
    Vector4 color2 = { 1, 1, 1, 1 };
    Vector2 size2 = { 30,50 };
    //スプライト一枚あたりのサイズ
    const Vector2 texsize2 = { 64,96 };
    Vector2 texBase2 = { 0,0 };

    // 位置
    Vector2 pos = { 800,200 };
};