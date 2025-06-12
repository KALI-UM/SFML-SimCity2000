#include "pch.h"
#include "Background_Lobby.h"
#include "DSprite.h"

Background_Lobby::Background_Lobby()
{
}

Background_Lobby::~Background_Lobby()
{
}

bool Background_Lobby::Initialize()
{
	m_Background = new DSprite("background/SimCity_2000_screen.png");
	m_Background->SetOriginCenter();


	SetDrawable(m_Background);



	return false;
}

void Background_Lobby::Reset()
{
	m_FadeOutSpeed = -1;
	m_CurrvColorValue = 150;
	m_Background->SetColor(m_CurrvColorValue, m_CurrvColorValue, m_CurrvColorValue);
}

void Background_Lobby::Update(float dt)
{
	sf::Vector2f viewSize = GAME_MGR->GetViewSize(0);
	sf::FloatRect viewRect = GAME_MGR->GetViewRect(0);

	setPosition(sf::Vector2f(viewSize.x / 2, viewSize.y / 2));
}

void Background_Lobby::FadeOutBackGround(float duration)
{
	m_FadeOutSpeed = 100 / duration;
}
