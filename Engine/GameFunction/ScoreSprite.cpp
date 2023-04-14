//#include "ScoreSprite.h"
//
//ScoreSprite::ScoreSprite()
//{
//}
//
//ScoreSprite::~ScoreSprite()
//{
//}
//
//void ScoreSprite::Initialize()
//{
//    Sprite::LoadTexture(10, L"Resources/123-export.png");
//    Sprite::LoadTexture(11, L"Resources/meternum.png");
//    for (int i = 0; i < 10; i++)
//    {
//        texBase.x = 80 * i;
//        scoresprite[i] = Sprite::Create(10, { pos.x,pos.y }, { color.x,color.y,color.z,color.w }, { anker.x,anker.y });
//        
//        scoresprite[i]->SetTextureRect({ texBase.x,texBase.y }, { texsize.x,texsize.y });
//        scoresprite[i]->SetSize({ size.x,size.y });
//    }
//    for (int i = 0; i < 10; i++)
//    {
//        texBase2.x = 64 * i;
//        scoresprite2[i] = Sprite::Create(11, { pos.x,pos.y }, { color.x,color.y,color.z,color.w }, { anker.x,anker.y });
//
//        scoresprite2[i]->SetTextureRect({ texBase2.x,texBase2.y }, { texsize2.x,texsize2.y });
//        scoresprite2[i]->SetSize({ size2.x,size2.y });
//    }
//}
//
//void ScoreSprite::Draw(int num, Vector2 pos)
//{
//    scoresprite[num]->SetPosition({ pos.x,pos.y });
//    scoresprite[num]->Draw();
//}
//void ScoreSprite::DrawSpeed(int num, Vector2 pos)
//{
//    scoresprite2[num]->SetPosition({ pos.x,pos.y });
//    scoresprite2[num]->Draw();
//}
//void ScoreSprite::SetSize(Vector2 size)
//{
//
//}