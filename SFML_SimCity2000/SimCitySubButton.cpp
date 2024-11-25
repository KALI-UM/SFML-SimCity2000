#include "pch.h"
#include "SimCitySubButton.h"
#include "DAtlasSprite.h"
#include "DText.h"

SimCitySubButton::SimCitySubButton(int bttCnt, int viewIndex)
	:m_ButtonCount(bttCnt), m_ViewIndex(viewIndex)
{
}

SimCitySubButton::~SimCitySubButton()
{
}

bool SimCitySubButton::Initialize()
{
	m_BarTexId = "ui/windowAtlas.png";
	m_Bar = new DAtlasSprite(m_BarTexId);
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 10);
	SetDrawable(m_Bar);

	m_FontId = "fonts/DOSGothic.ttf";
	m_Buttons.resize(m_ButtonCount);
	m_ButtonFunc.resize(m_ButtonCount);
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		DText* btt = new DText(m_FontId, "");
		btt->SetColor(ColorPalette::DarkGray);
		btt->SetCharacterSize(20);
		btt->setLocalPosition(sf::Vector2f(2, 2 + i * 22));

		m_Buttons[i] = btt;
		btt->SetPriorityType(DrawPriorityType::Custom, 11);
		SetDrawable(btt);
	}

	return false;
}

void SimCitySubButton::Reset()
{
	m_Bar->SetTexture(m_BarTexId);
	m_Bar->SetCornerSize(8);
	m_Bar->SetSize({ m_MaxSize, 22.0f * m_ButtonCount + 2.0f });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));
}

void SimCitySubButton::Update(float dt)
{
	if (m_Delay)
	{
		m_Delay = false;
		return;
	}

	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);
	int currIndex = -1;
	for (int i = 0; i < m_Buttons.size(); i++)
	{
		m_Buttons[i]->SetColor(ColorPalette::DarkGray);
		if (m_Buttons[i]->GetGlobalBounds().contains(currMousePos))
		{
			m_Buttons[i]->SetColor(ColorPalette::Black);
			currIndex = i;
		}
	}

	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left) || INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		if (currIndex != -1)
		{
			m_ButtonFunc[currIndex]();
		}
		ShowSubBar(false);
	}
}

void SimCitySubButton::Release()
{
}

void SimCitySubButton::ShowSubBar(bool value)
{
	SetIsValid(value);
	m_Bar->SetSize({ m_MaxSize, 22.0f * m_ButtonCount + 2.0f });
	m_Delay = value;
}

void SimCitySubButton::SetButtonText(int bttIndex, const std::wstring bttText)
{
	m_Buttons[bttIndex]->SetString(bttText);
	m_Buttons[bttIndex]->setLocalPosition(sf::Vector2f(2, 2 + bttIndex * 17));

	m_MaxSize = std::max(m_MaxSize, m_Buttons[bttIndex]->GetGlobalBounds().getSize().y);
}

void SimCitySubButton::SetButtonText(int bttIndex, const std::string bttText)
{
	m_Buttons[bttIndex]->SetString(bttText);
	m_Buttons[bttIndex]->setLocalPosition(sf::Vector2f(2, 2 + bttIndex * 17));
	m_MaxSize = std::max(m_MaxSize, m_Buttons[bttIndex]->GetGlobalBounds().getSize().x);
}
