#pragma once
#include "WorldTransform.h"
#include "Model.h"

#include "Input.h"
#include "ViewProjection.h"


#include "Defined.h"
MY_SUPPRESS_WARNINGS_BEGIN
#include <memory>
#include <list>
#include <cassert>
MY_SUPPRESS_WARNINGS_END

class BoxCollision {
public:
    //プレイヤーとエネミーの座標を入る
    //プレイヤーの半径をx,y,zの順に入れる
    //エネミーの半径をx,y,zの順に入れる
    bool BoxCollision_1(Vector3 player, Vector3 enemy, Vector3 playerWidth,Vector3 enemyWidth);
    bool BoxCollision_(Vector3 player, Vector3 playerWidth, Vector2 enemyPosX, Vector2 enemyPosY, Vector2 enemyPosZ);
    bool BoxCollision_2(Vector3 player, Vector3 enemy, Vector3 playerWidth, Vector3 enemyWidth);

    void Contact(int playerMove,WorldTransform enemy,Vector3 enemyRotationMove, Vector3 enemyMove);
private:
    //ワールド変換データ
    WorldTransform worldTransform;

    ////モデル
    //Model* model = nullptr;
    ////モデル
    //Model* enemyBulletModel = nullptr;
    
};
