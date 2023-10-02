#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "StageSelect.h"
#include "MediumBossStage.h"
#include "DebugScene.h"
#include "ClearScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // éüÇÃÉVÅ[ÉìÇê∂ê¨
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
    else if (sceneName == "ClearScene") {
        newScene = new ClearScene();
    }


    return newScene;
}
