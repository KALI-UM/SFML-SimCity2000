#include "pch.h"
#include "Scene_Loading.h"

Scene_Loading::~Scene_Loading()
{
}

bool Scene_Loading::Initialize()
{
    return true;
}

void Scene_Loading::Reset()
{
    m_StartTime = -1;
    m_PauseTime = 5;
}

void Scene_Loading::Update(float dt)
{
    if (m_StartTime < 0)
    {
        m_StartTime = FRAMEWORK->GetRealTime();
    }
    else if (m_StartTime + m_PauseTime > FRAMEWORK->GetRealTime())
    {

    }
}

void Scene_Loading::Release()
{
}
