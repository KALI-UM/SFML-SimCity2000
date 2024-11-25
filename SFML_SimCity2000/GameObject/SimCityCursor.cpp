#include "pch.h"
#include "SimCityCursor.h"
#include "SimCityGameSystem.h"
#include "SimCityButtonBar.h"

SimCityCursor::SimCityCursor(const std::string& texId, int viewIndex)
	:MouseCursor(texId, viewIndex)
{
}

SimCityCursor::~SimCityCursor()
{
}

bool SimCityCursor::Initialize()
{
	bool result = true;

	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			m_ModeTextureRect[j * 5 + i] = sf::IntRect(i * 32, j * 32, 32, 32);
		}
	}
	m_ModeTextureRect[(int)Action::PublicService] = sf::IntRect(4 * 32, 2 * 32, 32, 32);
	m_ModeTextureRect[(int)Action::Recreation] = sf::IntRect(0 * 32, 3 * 32, 32, 32);
	m_ModeTextureRect[(int)Action::ZoomOut] = sf::IntRect(3 * 32, 2 * 32, 32, 32);
	m_ModeTextureRect[(int)Action::Move] = sf::IntRect(1 * 32, 3 * 32, 32, 32);
	result &= MouseCursor::Initialize();
	return result;
}

void SimCityCursor::Reset()
{
	MouseCursor::Reset();
	m_CursorSprite->setScale({ 2,2 });
	SetCursorMode(Action::Move);
}

void SimCityCursor::Update(float dt)
{
	MouseCursor::Update(dt);
}

void SimCityCursor::SetCursorMode(Action mode)
{
	if (m_CurrentMode == mode)return;
	m_CurrentMode = mode;
	m_CursorSprite->SetTextureRect(m_ModeTextureRect[(int)mode]);
}
