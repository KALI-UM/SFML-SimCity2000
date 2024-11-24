#include "pch.h"
#include "SimCityMenuBar.h"
#include "DAtlasSprite.h"
#include "SimCityGameSystem.h"
#include "SimCitySubButton.h"
#include "SimCityCursor.h"

SimCityMenuBar::SimCityMenuBar(int viewIndex)
{
}

SimCityMenuBar::~SimCityMenuBar()
{
}

bool SimCityMenuBar::Initialize()
{
	m_BarTexId = "ui/windowAtlas.png";
	m_Bar = new DAtlasSprite(m_BarTexId);
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 1);
	SetDrawable(m_Bar);

    return false;
}

void SimCityMenuBar::Reset()
{

}

void SimCityMenuBar::Update(float dt)
{
}

void SimCityMenuBar::Release()
{
}

void SimCityMenuBar::SetSubButton(const Action& action, const std::vector<std::pair<Action, std::string>>& sub)
{
}
