#include "pch.h"
#include "Scene_SimCityMain.h"

Scene_SimCityMain::Scene_SimCityMain()
    :SceneBase("Main")
{
}

Scene_SimCityMain::~Scene_SimCityMain()
{
}

bool Scene_SimCityMain::Initialize()
{
    return false;
}

void Scene_SimCityMain::Reset()
{
}

void Scene_SimCityMain::Enter()
{
    SOUND_MGR->PlayBgm("sound/BGM/bgm.mp3", true, true, 30, 10);
}

void Scene_SimCityMain::Update(float dt)
{
    if (INPUT_MGR->GetKeyDown(sf::Keyboard::Enter))
    {
        SCENE_MGR->ChangeScene("InGame");
    }
}

void Scene_SimCityMain::Exit()
{
}
