#include "pch.h"
#include "DRectangle.h"

DRectangle::DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, const sf::Color& line, float thick, const sf::Color& fill)
	:DShape(&m_Rectangle, line, thick, fill)
{
	m_Rectangle.setPosition(position);
	SetSize(widthheight);
	SetOutlineColor(line);
	SetOutlineThickness(thick);
	SetFillColor(fill);
}

DRectangle::DRectangle(const sf::FloatRect& rect, const sf::Color& line, float thick, const sf::Color& fill)
	:DShape(&m_Rectangle, line, thick, fill)
{
	m_Rectangle.setPosition(rect.getPosition());
	SetSize(rect.getSize());
	SetOutlineColor(line);
	SetOutlineThickness(thick);
	SetFillColor(fill);
}

DRectangle::DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, sf::Texture* tex)
	:DShape(&m_Rectangle, tex)
{
	m_Rectangle.setPosition(position);
	SetSize(widthheight);
	if (tex)
	{
		m_IsValid = true;
	}
}

DRectangle::DRectangle(const sf::FloatRect& rect, sf::Texture* tex)
:DShape(&m_Rectangle, tex)
{
	m_Rectangle.setPosition(rect.getPosition());
	SetSize(rect.getSize());
	if (tex)
	{
		m_IsValid = true;
	}
}

DRectangle::DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, const std::string& filepath)
	:DShape(&m_Rectangle, ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath))
{
	m_Rectangle.setPosition(position);
	SetSize(widthheight);
}

DRectangle::DRectangle(const sf::FloatRect& rect, const std::string& filepath)
	:DShape(&m_Rectangle, ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath))
{
	m_Rectangle.setPosition(rect.getPosition());
	SetSize(rect.getSize());
}

DRectangle::DRectangle(const DRectangle& other)
	:DShape(other)
{
}

DRectangle::DRectangle(DRectangle&& other)
	:DShape(other)
{
}

DRectangle::~DRectangle()
{
}

void DRectangle::SetSize(const sf::Vector2f& widthheight)
{
	m_Rectangle.setSize(widthheight);
	m_FloatRect.left = GetBoundPointPosition(0).x;
	m_FloatRect.top = GetBoundPointPosition(0).y;
	m_FloatRect.width = widthheight.x;
	m_FloatRect.height = widthheight.y;
}

sf::FloatRect DRectangle::GetGlobalBounds() const
{
	return m_FloatRect;
}

void DRectangle::SetFloatRect(const sf::FloatRect& frect)
{
	m_Rectangle.setPosition(frect.getPosition()+m_Rectangle.getOrigin());
	SetSize(frect.getSize());
	m_FloatRect = frect;
}

sf::FloatRect DRectangle::GetLocalBounds() const
{
	return m_Rectangle.getLocalBounds();
}
