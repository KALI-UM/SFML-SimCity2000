#include "pch.h"
#include "GameStart.h"
#include "DText.h"

GameStart::GameStart()
{
}

GameStart::~GameStart()
{
}

bool GameStart::Initialize()
{
	m_GameStart = new DText("resource/CookieRun Bold.ttf", "CLICK TO GAME START!!", 40);
	m_GameStart->SetColor(sf::Color::White);
	//m_GameStart->SetOutlineColor(sf::Color::Black);
	m_GameStart->SetOutlineThickness(3);
	SetDrawable(m_GameStart);
	return false;
}

void GameStart::Reset()
{
	m_GameStart->SetOriginCenter();
	m_GameStart->setPosition(GAME_MGR->GetWindow()->getSize().x / 2, GAME_MGR->GetWindow()->getSize().y - 150);
}

void GameStart::Update(float dt)
{
}
