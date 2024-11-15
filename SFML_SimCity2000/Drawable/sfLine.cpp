#include "pch.h"
#include "sfLine.h"

sfLine::sfLine()
{
}

sfLine::~sfLine()
{
}

void sfLine::setColor(const sf::Color& color)
{
	m_Color = color;
	for (int i = 0; i < m_Vertices.getVertexCount(); i++)
	{
		m_Vertices[i].color = color;
	}
}

const sf::Color& sfLine::getColor() const
{
	return m_Color;
}

sf::FloatRect sfLine::getGlobalBounds() const
{
	return getTransform().transformRect(m_Vertices.getBounds());
}

sf::FloatRect sfLine::getLocalBounds() const
{
	return m_Vertices.getBounds();
}

void sfLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	target.draw(m_Vertices, states);
}