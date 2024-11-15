#include "pch.h"
#include "DShape.h"

DShape::DShape(sf::Shape* shape, const sf::Color& line, float thick, const sf::Color& fill)
	:m_Shape(shape), DrawableObject(DataType::Shape, shape, shape)
{
	m_IsValid = m_Shape;
}

DShape::DShape(sf::Shape* shape, sf::Texture* tex)
	:m_Shape(shape), DrawableObject(DataType::Shape, shape, shape)
{
	m_IsValid = m_Shape;
	SetTexture(tex);
}

DShape::DShape(const DShape& other)
	:DrawableObject(other, m_Shape, m_Shape)
{
}

DShape::DShape(DShape&& other)
	:DrawableObject(other, m_Shape, m_Shape)
{
}

DShape::~DShape()
{
}

void DShape::SetTexture(sf::Texture* tex)
{
	m_Shape->setTexture(tex);
	if (tex)
	{
		SetFillColor(sf::Color::Transparent);
	}
}

sf::Vector2u DShape::GetTextureSize() const
{
	return m_Shape->getTexture() ? m_Shape->getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::Color DShape::GetColor() const
{
	return m_Shape->getFillColor();
}

void DShape::SetColor(const sf::Color& color)
{
	SetFillColor(color);
}

void DShape::SetColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

sf::Color DShape::GetFillColor() const
{
	return m_Shape->getFillColor();
}

sf::Color DShape::GetOutlineColor() const
{
	return m_Shape->getOutlineColor();
}

void DShape::SetFillColor(const sf::Color& color)
{
	m_Shape->setFillColor(color);
}

void DShape::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DShape::SetOutlineColor(const sf::Color& color)
{
	m_Shape->setOutlineColor(color);
}

void DShape::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}

void DShape::SetOutlineThickness(float thick)
{
	m_Shape->setOutlineThickness(thick);
}
