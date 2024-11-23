#include "pch.h"
#include "SimCityButtonBar.h"
#include "DAtlasSprite.h"
#include "SimCityGameSystem.h"
#include "SimCityCursor.h"

SimCityButtonBar::SimCityButtonBar(int viewIndex)
	:m_ViewIndex(viewIndex)
{
}

SimCityButtonBar::~SimCityButtonBar()
{
}

bool SimCityButtonBar::Initialize()
{
	m_BarTexId = "ui/windowAtlas.png";
	m_Bar = new DAtlasSprite(m_BarTexId);
	SetDrawable(m_Bar);

	m_ButtonTexId = "ui/button.png";
	m_Buttons.resize(18);
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		DSprite* btt = new DSprite(m_ButtonTexId);

		btt->setLocalPosition(sf::Vector2f(i % 3, i / 3) * 32.0f + sf::Vector2f(10, 32));

		m_Buttons[i] = btt;
		SetDrawable(btt);
	}


	m_ButtonWork.resize(18);
	m_ButtonWork[(int)ButtonName::Destroy] = true;
	m_ButtonWork[(int)ButtonName::Tree] = false;
	m_ButtonWork[(int)ButtonName::Alram] = false;
	m_ButtonWork[(int)ButtonName::Elec] = true;
	m_ButtonWork[(int)ButtonName::Water] = false;
	m_ButtonWork[(int)ButtonName::Religion] = false;
	m_ButtonWork[(int)ButtonName::Road] = true;
	m_ButtonWork[(int)ButtonName::Rail] = false;
	m_ButtonWork[(int)ButtonName::Port] = false;
	m_ButtonWork[(int)ButtonName::ZoneResidential] = true;
	m_ButtonWork[(int)ButtonName::ZoneCommercial] = true;
	m_ButtonWork[(int)ButtonName::ZoneIndustrial] = true;
	m_ButtonWork[(int)ButtonName::Education] = false;
	m_ButtonWork[(int)ButtonName::PoliceStation] = false;
	m_ButtonWork[(int)ButtonName::Park] = false;
	m_ButtonWork[(int)ButtonName::ZoomIn] = false;
	m_ButtonWork[(int)ButtonName::ZoomOut] = false;
	m_ButtonWork[(int)ButtonName::Move] = false;

	return false;
}

void SimCityButtonBar::Reset()
{
	m_Bar->SetTexture(m_BarTexId);
	m_Bar->SetCornerSize(32);
	m_Bar->SetSize({ 32 * 3 + 20, 300 });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));

	m_Buttons[(int)ButtonName::Destroy]->SetTextureRect(sf::IntRect(328, 368, 32, 32));
	m_Buttons[(int)ButtonName::Tree]->SetTextureRect(sf::IntRect(328 + 40, 368, 32, 32));
	m_Buttons[(int)ButtonName::Alram]->SetTextureRect(sf::IntRect(328 + 40 * 2, 368, 32, 32));
	m_Buttons[(int)ButtonName::Elec]->SetTextureRect(sf::IntRect(328 + 40 * 3, 368, 32, 32));
	m_Buttons[(int)ButtonName::Water]->SetTextureRect(sf::IntRect(8, 408, 32, 32));
	m_Buttons[(int)ButtonName::Religion]->SetTextureRect(sf::IntRect(8 + 40, 408, 32, 32));
	m_Buttons[(int)ButtonName::Road]->SetTextureRect(sf::IntRect(8 + 40 * 2, 408, 32, 32));
	m_Buttons[(int)ButtonName::Rail]->SetTextureRect(sf::IntRect(8 + 40 * 3, 408, 32, 32));
	m_Buttons[(int)ButtonName::Port]->SetTextureRect(sf::IntRect(8 + 40 * 4, 408, 32, 32));
	m_Buttons[(int)ButtonName::ZoneResidential]->SetTextureRect(sf::IntRect(8 + 40 * 5, 408, 32, 32));
	m_Buttons[(int)ButtonName::ZoneCommercial]->SetTextureRect(sf::IntRect(8 + 40 * 6, 408, 32, 32));
	m_Buttons[(int)ButtonName::ZoneIndustrial]->SetTextureRect(sf::IntRect(8 + 40 * 7, 408, 32, 32));
	m_Buttons[(int)ButtonName::Education]->SetTextureRect(sf::IntRect(328, 208, 32, 32));
	m_Buttons[(int)ButtonName::PoliceStation]->SetTextureRect(sf::IntRect(368, 408, 32, 32));
	m_Buttons[(int)ButtonName::Park]->SetTextureRect(sf::IntRect(368 + 40, 408, 32, 32));
	m_Buttons[(int)ButtonName::ZoomIn]->SetTextureRect(sf::IntRect(328, 288, 32, 32));
	m_Buttons[(int)ButtonName::ZoomOut]->SetTextureRect(sf::IntRect(328 + 40, 288, 32, 32));
	m_Buttons[(int)ButtonName::Move]->SetTextureRect(sf::IntRect(168, 448, 32, 32));

	for (int i = 0; i < m_Buttons.size(); i++)
	{
		if (!m_ButtonWork[i])
		{
			m_Buttons[i]->SetColor(ColorPalette::DarkRed);
		}
	}
}

void SimCityButtonBar::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);

	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_Cursor->SetCursorMode(ButtonName::NotUse22);
	}
	else if (m_Cursor->GetCursorMode() != (ButtonName)m_CurrButtonIndex)
	{
		m_Cursor->SetCursorMode((ButtonName)m_CurrButtonIndex);
	}

	for (int i = 0; i < m_Buttons.size(); i++)
	{
		if (m_ButtonWork[i])
		{
			if (m_Buttons[i]->GetGlobalBounds().contains(currMousePos) && INPUT_MGR->GetMouseDown(sf::Mouse::Left))
			{
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::White);

				SOUND_MGR->PlaySfx("sound/SFX/Click.wav");
				m_CurrButtonIndex = i;
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::Gray);
				m_Cursor->SetCursorMode((ButtonName)m_CurrButtonIndex);

				m_ButtonFunc((ButtonName)m_CurrButtonIndex);
			}
		}
	}
}

void SimCityButtonBar::Release()
{
}
