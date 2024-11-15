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
	m_Logo = new DSprite("ui/logo.png");
	m_Background[0] = new DSprite("background/001.png");
	m_Background[1] = new DSprite("background/001.png");
	m_LobbyCharacter = new DSprite("ui/character_01.png");

	SetDrawable(m_Background[0]);
	SetDrawable(m_Background[1]);
	SetDrawable(m_LobbyCharacter);
	SetDrawable(m_Logo);
	return false;
}

void Background_Lobby::Reset()
{
	m_Logo->SetOriginCenter();
	m_Logo->setPosition(GAME_MGR->GetWindow()->getSize().x / 2, GAME_MGR->GetWindow()->getSize().y / 4);
	m_LobbyCharacter->setScale(0.5f, 0.5f);
	m_LobbyCharacter->SetOriginCenter();
	m_LobbyCharacter->setPosition(GAME_MGR->GetWindow()->getSize().x / 2, GAME_MGR->GetWindow()->getSize().y / 2);
	sf::Vector2f v1 = m_LobbyCharacter->getOrigin();
	sf::Vector2f v2 = m_LobbyCharacter->getPosition();
	m_FadeOutSpeed = -1;
	m_CurrvColorValue = 150;
	m_Background[0]->SetColor(m_CurrvColorValue, m_CurrvColorValue, m_CurrvColorValue);
	m_Background[1]->SetColor(m_CurrvColorValue, m_CurrvColorValue, m_CurrvColorValue);
}

void Background_Lobby::Update(float dt)
{

	if (m_FadeOutSpeed > 0)
	{
		//std::cout << "global " << MOUSEPOS.x << "," << MOUSEPOS.y << std::endl;
		//std::cout << "view " << MOUSEVIEWPOS.x << "," << MOUSEVIEWPOS.y << std::endl;

		m_CurrvColorValue = m_CurrvColorValue + (dt * m_FadeOutSpeed);
		m_Background[0]->SetColor(m_CurrvColorValue, m_CurrvColorValue, m_CurrvColorValue);
		m_Background[1]->SetColor(m_CurrvColorValue, m_CurrvColorValue, m_CurrvColorValue);

		if (m_CurrvColorValue >= 255)
		{
			SCENE_MGR->ChangeScene("Play1");
		}
	}
}

void Background_Lobby::FadeOutBackGround(float duration)
{
	m_FadeOutSpeed = 100 / duration;
}
