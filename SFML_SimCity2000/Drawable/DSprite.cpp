#include "pch.h"
#include "DSprite.h"

DSprite::DSprite(sf::Texture* tex)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DSprite::DSprite(const std::string& filepath)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DSprite::DSprite(const sf::Vector2f& position, sf::Texture* tex)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	m_Sprite.setPosition(position);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DSprite::DSprite(const sf::Vector2f& position, const std::string& filepath)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	m_Sprite.setPosition(position);
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DSprite::DSprite()
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
}

DSprite::DSprite(const DSprite& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

DSprite::DSprite(DSprite&& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

DSprite::~DSprite()
{
}

sf::FloatRect DSprite::GetLocalBounds() const
{
	return m_Sprite.getLocalBounds();
}

sf::FloatRect DSprite::GetGlobalBounds() const
{
	return m_Sprite.getGlobalBounds();
}

void DSprite::SetTexture(sf::Texture* tex, bool resetRect)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex, resetRect);
	}
}

void DSprite::SetTexture(const std::string& filepath, bool resetRect)
{
	SetTexture(ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath),resetRect);
}

void DSprite::SetTextureRect(const sf::IntRect& rect)
{
	m_Sprite.setTextureRect(rect);
}

void DSprite::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	setOrigin(((GetTextureSize().x / 2) * ((int)type % 3)) + detail.x,
		((GetTextureSize().y / 2) * ((int)type / 3)) + detail.y);
}

sf::Vector2u DSprite::GetTextureSize() const
{
	return m_Sprite.getTexture() ? m_Sprite.getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::Color DSprite::GetColor() const
{
	return m_Sprite.getColor();
}

void DSprite::SetColor(const sf::Color& color)
{
	m_Sprite.setColor(color);
}

void DSprite::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color DSprite::GetFillColor() const
{
	return GetColor();
}

sf::Color DSprite::GetOutlineColor() const
{
	return GetColor();
}

void DSprite::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void DSprite::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DSprite::SetOutlineColor(const sf::Color& color)
{
	SetColor(color);
}

void DSprite::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}
