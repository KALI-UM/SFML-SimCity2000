#include "pch.h"
#include "DAtlasSprite.h"

DAtlasSprite::DAtlasSprite(sf::Texture* tex)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DAtlasSprite::DAtlasSprite(const std::string& filepath)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

DAtlasSprite::DAtlasSprite()
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
}

DAtlasSprite::DAtlasSprite(const DAtlasSprite& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

DAtlasSprite::DAtlasSprite(DAtlasSprite&& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

DAtlasSprite::~DAtlasSprite()
{
}

sf::FloatRect DAtlasSprite::GetLocalBounds() const
{
	return m_Sprite.getLocalBounds();
}

sf::FloatRect DAtlasSprite::GetGlobalBounds() const
{
	return m_Sprite.getGlobalBounds();
}

void DAtlasSprite::SetTexture(sf::Texture* tex)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

void DAtlasSprite::SetTexture(const std::string& filepath)
{
	SetTexture(ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath));
}

void DAtlasSprite::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	setOrigin(((GetTextureSize().x / 2) * ((int)type % 3)) + detail.x,
		((GetTextureSize().y / 2) * ((int)type / 3)) + detail.y);
}

sf::Vector2u DAtlasSprite::GetTextureSize() const
{
	return m_Sprite.getTexture() ? m_Sprite.getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::Color DAtlasSprite::GetColor() const
{
	return m_Sprite.getColor();
}

void DAtlasSprite::SetColor(const sf::Color& color)
{
	m_Sprite.setColor(color);
}

void DAtlasSprite::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color DAtlasSprite::GetFillColor() const
{
	return GetColor();
}

sf::Color DAtlasSprite::GetOutlineColor() const
{
	//return GetColor();
	return sf::Color::Transparent;
}

void DAtlasSprite::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void DAtlasSprite::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DAtlasSprite::SetOutlineColor(const sf::Color& color)
{
	//SetColor(color);
}

void DAtlasSprite::SetOutlineColor(int r, int g, int b, int a)
{
	//SetOutlineColor(sf::Color(r, g, b, a));
}

void DAtlasSprite::SetSize(const sf::Vector2f& size)
{
	m_Sprite.setSize(size);
}

void DAtlasSprite::SetCornerSize(float size)
{
	m_Sprite.setCornerSize(size);
}

void DAtlasSprite::SetAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR, const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR, const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR)
{
	m_Sprite.setAtlas(	TL, TC, TR,
						ML, MC, MR,
						BL, BC, BR);
}

