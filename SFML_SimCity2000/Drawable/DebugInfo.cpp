#include "pch.h"
#include "DebugInfo.h"

DebugInfo::DebugInfo(const sf::FloatRect& target, const sf::Transform& transform, sf::Vector2f& pos)
	: m_Position(pos)
{
	m_Rectangle.setFillColor(sf::Color::Transparent);
	m_Rectangle.setOutlineColor(sf::Color::Green);
	m_Rectangle.setOutlineThickness(1);
	m_X.resize(4);
	m_X.setPrimitiveType(sf::Lines);
	m_X[0].position = { + 4,+ 4 };
	m_X[1].position = { - 4,- 4 };
	m_X[2].position = { + 4,- 4 };
	m_X[3].position = { - 4,+ 4 };
	setXColor(sf::Color::Magenta);
	Update(target, transform);
}

DebugInfo::~DebugInfo()
{
}

void DebugInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform = m_RectTransform;
	target.draw(m_Rectangle, states);
	states.transform = m_XTransform; // getTransform() is defined by sf::Transformable
	target.draw(m_X, states);
}

void DebugInfo::Update(const sf::FloatRect& localBound, const sf::Transform& transform)
{
	m_Rectangle.setSize(localBound.getSize());
	m_Rectangle.setPosition(localBound.getPosition());
	m_RectTransform = transform;
	m_XTransform = sf::Transform::Identity;
	m_XTransform.translate(m_Position);
}

void DebugInfo::setColor(const sf::Color& color)
{
	setOutlineColor(color);
}

void DebugInfo::setColor(int r, int g, int b, int a)
{
	setColor(sf::Color(r, g, b, a));
}

const sf::Color& DebugInfo::getColor() const
{
	return getRectColor();
}

void DebugInfo::setOutlineColor(const sf::Color& color)
{
	m_Rectangle.setOutlineColor(color);
}

void DebugInfo::setOutlineColor(int r, int g, int b, int a)
{
	setOutlineColor(sf::Color(r, g, b, a));
}

const sf::Color& DebugInfo::getRectColor() const
{
	return m_Rectangle.getOutlineColor();
}

void DebugInfo::setXColor(const sf::Color& color)
{
	m_XColor = color;
	for (int i = 0; i < m_X.getVertexCount(); i++)
	{
		m_X[i].color = m_XColor;
	}
}

void DebugInfo::setXColor(int r, int g, int b, int a)
{
	setXColor(sf::Color(r, g, b, a));
}

const sf::Color& DebugInfo::getXColor() const
{
	return m_XColor;
}
