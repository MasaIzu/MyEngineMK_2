#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "StageSelect.h"

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

    return newScene;
}
