#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "StageSelect.h"
#include "MediumBossStage.h"
#include "DebugScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーンを生成
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "STAGESELECT") {
        newScene = new StageSelect();
    }
    else if (sceneName == "GAMEPLAY") {
        newScene = new GameScene();
    }
    else if (sceneName == "STAGE2") {
        newScene = new MediumBossStage();
    }
    else if (sceneName == "Debug") {
        newScene = new DebugScene();
    }

    return newScene;
}
