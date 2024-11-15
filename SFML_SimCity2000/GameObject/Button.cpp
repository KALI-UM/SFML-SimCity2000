#include "pch.h"
#include "Button.h"

Button::Button()
{
}

Button::~Button()
{
}

bool Button::Initialize()
{
	m_ButtonSprite = new DSprite("ui/button.png");
	m_ButtonSprite->SetOriginCenter();
	m_ButtonText = new DText("resource/CookieRun Black.ttf", "", 30);
	m_ButtonText->SetOutlineColor(sf::Color::Black);
	m_ButtonText->SetOutlineThickness(1);
	m_ButtonText->SetOriginCenter();
	SetDrawable(m_ButtonSprite);
	SetDrawable(m_ButtonText);

	return true;
}

void Button::Reset()
{
	m_ButtonSprite->SetColor(m_DefaultColor);
	m_ButtonArea = m_ButtonSprite->GetGlobalBounds();
	m_IsOverlaying = false;
	m_IsClicked = false;
}

void Button::Update(float dt)
{
	m_IsOverlaying = m_ButtonArea.contains({ (float)MOUSEPOS.x,(float)MOUSEPOS.y });
	m_IsClicked = m_IsOverlaying && INPUT_MGR->GetMouseDown(sf::Mouse::Left);

	if (m_IsOverlaying)
	{
		m_ButtonSprite->SetColor(m_OverlayColor);
		if (m_ButtonSprite->GetDebugDraw())
			m_ButtonSprite->GetDebugDraw()->setColor(ColorPalette::Red);
		if (m_WhenOverlay)
			m_WhenOverlay();
	}
	else
	{
		m_ButtonSprite->SetColor(m_DefaultColor);
		if (m_ButtonSprite->GetDebugDraw())
			m_ButtonSprite->GetDebugDraw()->setColor(ColorPalette::Blue);
	}

	if (m_IsClicked)
	{
		if (m_WhenClicked)
			m_WhenClicked();
	}
}

void Button::SetButtonPosition(const sf::Vector2f& pos)
{
	m_ButtonSprite->setPosition(pos);
	m_ButtonText->setPosition(pos);
	m_ButtonArea = m_ButtonSprite->GetGlobalBounds();
}

void Button::SetButtonText(const std::string& text)
{
	m_ButtonText->SetString(text);
	m_ButtonText->SetOriginCenter();
}

void Button::SetButtonColor(const sf::Color& on, const sf::Color& off)
{
	m_DefaultColor = off;
	m_OverlayColor = on;
}

bool Button::GetIsOveraying() const
{
	return m_IsOverlaying;
}

bool Button::GetIsClicked() const
{
	return m_IsClicked;
}

void Button::SetOverlayFunc(std::function<void()> func)
{
	m_WhenOverlay = func;
}

void Button::SetClickedFunc(std::function<void()> func)
{
	m_WhenClicked = func;
}
