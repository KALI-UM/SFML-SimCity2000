#include "pch.h"
#include "SimCityMenuBar.h"
#include "DAtlasSprite.h"
#include "SimCityGameSystem.h"
#include "SimCitySubButton.h"
#include "SimCityCursor.h"

SimCityMenuBar::SimCityMenuBar(int viewIndex)
	:m_ViewIndex(viewIndex)
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
	m_FontId = "fonts/DOSGothic.ttf";
	m_Buttons.resize(m_ButtonCount);
	for (int i = 0; i < m_ButtonCount; i++)
	{
		DText* btt = new DText(m_FontId, "");
		btt->SetColor(ColorPalette::DarkGray);
		btt->SetCharacterSize(20);
		btt->setLocalPosition(sf::Vector2f(10 + i * 100, 2));

		m_Buttons[i] = btt;
		btt->SetPriorityType(DrawPriorityType::Custom, 2);
		SetDrawable(btt);
	}

	m_SubButtons.resize(m_ButtonCount);
	return false;
}

void SimCityMenuBar::Reset()
{
	m_Bar->SetTexture(m_BarTexId);
	m_Bar->SetCornerSize(8);
	m_Bar->SetSize({ (float)GAME_MGR->GetWindow()->getSize().x, 24 });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));

	while (!m_WantsToAddSubButtons.empty())
	{
		SimCitySubButton* subbtt = m_WantsToAddSubButtons.front();
		m_WantsToAddSubButtons.pop();
		subbtt->ShowSubBar(false);
		SCENE_MGR->GetCurrentScene()->AddGameObject(SCENE_MGR->GetCurrentScene()->m_UILayerIndex, subbtt);
		SetChildObj(subbtt);
	}
}

void SimCityMenuBar::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);
	m_HasFocus = false;
	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_HasFocus = true;
	}

	for (int i = 0; i < m_ButtonCount; i++)
	{
		if (m_Buttons[i]->GetGlobalBounds().contains(currMousePos) && INPUT_MGR->GetMouseDown(sf::Mouse::Left))
		{
			SOUND_MGR->PlaySfx("sound/SFX/Click.wav");
			m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::DarkGray);
			m_CurrButtonIndex = i;
			m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::Black);

			if (m_SubButtons[i])
			{
				m_SubButtons[i]->ShowSubBar(true);
				m_SubButtons[i]->setLocalPosition(m_Buttons[i]->getLocalPosition() + sf::Vector2f(0, 24));
			}
		}
	}
}

void SimCityMenuBar::Release()
{
}

void SimCityMenuBar::SetMenuString(const Menu& menu, const std::string& name)
{
	m_Buttons[(int)menu]->SetString(name);
}

void SimCityMenuBar::SetSubButton(const Menu& menu, const std::vector<std::pair<Menu, MenuSet&>>& sub)
{
	if (sub.size() == 0)
		return;

	SimCitySubButton* subbtt = new SimCitySubButton(sub.size(), m_ViewIndex);
	subbtt->INITIALIZE();
	subbtt->RESET();

	for (int i = 0; i < sub.size(); i++)
	{
		subbtt->SetButtonText(i, sub[i].second.name);
		if (sub[i].second.func)
			subbtt->SetButtonFunc(i, sub[i].second.func);
	}
	m_WantsToAddSubButtons.push(subbtt);
	m_SubButtons[(int)menu] = subbtt;
}
