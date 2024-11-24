#include "pch.h"
#include "SimCityButtonBar.h"
#include "DAtlasSprite.h"
#include "SimCityGameSystem.h"
#include "SimCitySubButton.h"
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
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 1);
	SetDrawable(m_Bar);

	m_ButtonTexId = "ui/button.png";
	m_Buttons.resize(m_ButtonCount);
	for (int i = 0; i < m_ButtonCount; i++)
	{
		DSprite* btt = new DSprite(m_ButtonTexId);

		btt->setLocalPosition(sf::Vector2f(i % 3, i / 3) * 32.0f + sf::Vector2f(10, 32));
		m_Buttons[i] = btt;
		btt->SetPriorityType(DrawPriorityType::Custom, 2);
		SetDrawable(btt);
	}

	m_ButtonWork.resize(m_ButtonCount);
	m_ButtonWork[(int)Action::Bulldozer] = true;
	m_ButtonWork[(int)Action::Landscape] = false;
	m_ButtonWork[(int)Action::Dispatch] = false;
	m_ButtonWork[(int)Action::PowerSupply] = true;
	m_ButtonWork[(int)Action::WaterSupply] = false;
	m_ButtonWork[(int)Action::Religion] = false;
	m_ButtonWork[(int)Action::Road] = true;
	m_ButtonWork[(int)Action::Rail] = false;
	m_ButtonWork[(int)Action::Port] = false;
	m_ButtonWork[(int)Action::ZoneResidential] = true;
	m_ButtonWork[(int)Action::ZoneCommercial] = true;
	m_ButtonWork[(int)Action::ZoneIndustrial] = true;
	m_ButtonWork[(int)Action::Education] = true;
	m_ButtonWork[(int)Action::PublicService] = true;
	m_ButtonWork[(int)Action::Recreation] = true;
	m_ButtonWork[(int)Action::ZoomIn] = true;
	m_ButtonWork[(int)Action::ZoomOut] = true;
	m_ButtonWork[(int)Action::Move] = true;

	m_SubButtons.resize(m_ButtonCount);

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

	m_Buttons[(int)Action::Bulldozer]->SetTextureRect(sf::IntRect(328, 368, 32, 32));
	m_Buttons[(int)Action::Landscape]->SetTextureRect(sf::IntRect(328 + 40, 368, 32, 32));
	m_Buttons[(int)Action::Dispatch]->SetTextureRect(sf::IntRect(328 + 40 * 2, 368, 32, 32));
	m_Buttons[(int)Action::PowerSupply]->SetTextureRect(sf::IntRect(328 + 40 * 3, 368, 32, 32));
	m_Buttons[(int)Action::WaterSupply]->SetTextureRect(sf::IntRect(8, 408, 32, 32));
	m_Buttons[(int)Action::Religion]->SetTextureRect(sf::IntRect(8 + 40, 408, 32, 32));
	m_Buttons[(int)Action::Road]->SetTextureRect(sf::IntRect(8 + 40 * 2, 408, 32, 32));
	m_Buttons[(int)Action::Rail]->SetTextureRect(sf::IntRect(8 + 40 * 3, 408, 32, 32));
	m_Buttons[(int)Action::Port]->SetTextureRect(sf::IntRect(8 + 40 * 4, 408, 32, 32));
	m_Buttons[(int)Action::ZoneResidential]->SetTextureRect(sf::IntRect(8 + 40 * 5, 408, 32, 32));
	m_Buttons[(int)Action::ZoneCommercial]->SetTextureRect(sf::IntRect(8 + 40 * 6, 408, 32, 32));
	m_Buttons[(int)Action::ZoneIndustrial]->SetTextureRect(sf::IntRect(8 + 40 * 7, 408, 32, 32));
	m_Buttons[(int)Action::Education]->SetTextureRect(sf::IntRect(328, 208, 32, 32));
	m_Buttons[(int)Action::PublicService]->SetTextureRect(sf::IntRect(368, 408, 32, 32));
	m_Buttons[(int)Action::Recreation]->SetTextureRect(sf::IntRect(368 + 40, 408, 32, 32));
	m_Buttons[(int)Action::ZoomIn]->SetTextureRect(sf::IntRect(328, 288, 32, 32));
	m_Buttons[(int)Action::ZoomOut]->SetTextureRect(sf::IntRect(328 + 40, 288, 32, 32));
	m_Buttons[(int)Action::Move]->SetTextureRect(sf::IntRect(168, 448, 32, 32));

	for (int i = 0; i < m_ButtonCount; i++)
	{
		if (!m_ButtonWork[i])
		{
			m_Buttons[i]->SetColor(ColorPalette::DarkRed);
		}
	}

	while (!m_WantsToAddSubButtons.empty())
	{
		SimCitySubButton* subbtt = m_WantsToAddSubButtons.front();
		m_WantsToAddSubButtons.pop();
		subbtt->ShowSubBar(false);
		SCENE_MGR->GetCurrentScene()->AddGameObject(SCENE_MGR->GetCurrentScene()->m_UILayerIndex, subbtt);
		SetChildObj(subbtt);
	}

	//setScale({ 2,2 });
}

void SimCityButtonBar::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);
	m_HasFocus = false;
	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_HasFocus = true;
	}

	for (int i = 0; i < m_ButtonCount; i++)
	{
		if (m_ButtonWork[i])
		{
			if (m_Buttons[i]->GetGlobalBounds().contains(currMousePos) && INPUT_MGR->GetMouseDown(sf::Mouse::Left))
			{
				SOUND_MGR->PlaySfx("sound/SFX/Click.wav");
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::White);
				m_CurrButtonIndex = i;
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::Gray);
				m_ButtonFunc((Action)m_CurrButtonIndex);

				if (m_SubButtons[i])
				{
					m_SubButtons[i]->ShowSubBar(true);
					m_SubButtons[i]->setLocalPosition(m_Buttons[i]->getLocalPosition() + sf::Vector2f(0, 32));
				}
			}
		}
	}
}

void SimCityButtonBar::Release()
{
}

void SimCityButtonBar::SetSubButton(const Action& action, const std::vector<std::pair<Action, std::string>>& sub)
{
	if (sub.size() == 0)
		return;

	SimCitySubButton* subbtt = new SimCitySubButton(sub.size(), m_ViewIndex);
	subbtt->INITIALIZE();
	subbtt->RESET();
	for (int i = 0; i < sub.size(); i++)
	{
		subbtt->SetButtonText(i, sub[i].second);
		subbtt->SetButtonFunc(i, [=]() { this->m_ButtonFunc(sub[i].first); });
	}
	m_WantsToAddSubButtons.push(subbtt);
	m_SubButtons[(int)action] = subbtt;
}
