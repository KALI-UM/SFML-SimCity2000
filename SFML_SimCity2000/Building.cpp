#include "pch.h"
#include "Building.h"
#include "SimCityGameSystem.h"

Building::~Building()
{
}

void Building::Reset()
{
    if (!m_GameSystem)
        SetIsValid(false);
}

void Building::Update(float dt)
{
    GameSysSpeedUpdate(m_GameSystem->GetGameSysSpeed() * dt);
}


