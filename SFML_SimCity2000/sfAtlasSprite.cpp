#include "pch.h"
#include "sfAtlasSprite.h"

sfAtlasSprite::sfAtlasSprite()
{
	m_Vertices.resize(4);
	for (int i = 0; i < 3; i++)
	{
		m_Atlas[i].resize(12);
		m_Atlas[i].setPrimitiveType(sf::Quads);
	}
}

void sfAtlasSprite::setTexture(const sf::Texture& tex)
{
	m_Texture = &tex;
}

const sf::Texture* sfAtlasSprite::getTexture() const
{
	return m_Texture;
}

void sfAtlasSprite::setColor(const sf::Color& color)
{
	m_Color = color;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < m_Atlas[j].getVertexCount(); i++)
		{
			m_Atlas[j][i].color = color;
		}
	}
}

const sf::Color& sfAtlasSprite::getColor() const
{
	return m_Color;
}

sf::FloatRect sfAtlasSprite::getGlobalBounds() const
{
	return getTransform().transformRect(m_Vertices.getBounds());
}

sf::FloatRect sfAtlasSprite::getLocalBounds() const
{
	return m_Vertices.getBounds();
}

void sfAtlasSprite::setSize(const sf::Vector2f& size)
{
	if (size.x < m_CornerSize * 2 || size.y < m_CornerSize * 2)return;

	m_Size = size;
	m_Vertices[1].position = { 0,m_Size.y };
	m_Vertices[2].position = { m_Size.x,0 };
	m_Vertices[3].position = m_Size;

	sf::FloatRect lot = { 0,0,m_CornerSize, m_CornerSize };

	m_Atlas[0][0].position = { 0,						0 };
	m_Atlas[0][1].position = { m_CornerSize,			0 };
	m_Atlas[0][2].position = { m_CornerSize,			m_CornerSize };
	m_Atlas[0][3].position = { 0,						m_CornerSize };
	m_Atlas[0][4].position = { m_CornerSize,			0 };
	m_Atlas[0][5].position = { m_Size.x - m_CornerSize,	0 };
	m_Atlas[0][6].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Atlas[0][7].position = { m_CornerSize,			m_CornerSize };
	m_Atlas[0][8].position = { m_Size.x - m_CornerSize,	0 };
	m_Atlas[0][9].position = { m_Size.x,				0 };
	m_Atlas[0][10].position = { m_Size.x,				m_CornerSize };
	m_Atlas[0][11].position = { m_Size.x - m_CornerSize,m_CornerSize };

	m_Atlas[1][0].position = { 0,						m_CornerSize };
	m_Atlas[1][1].position = { m_CornerSize,			m_CornerSize };
	m_Atlas[1][2].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Atlas[1][3].position = { 0,						m_Size.y - m_CornerSize };
	m_Atlas[1][4].position = { m_CornerSize,			m_CornerSize };
	m_Atlas[1][5].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Atlas[1][6].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Atlas[1][7].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Atlas[1][8].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Atlas[1][9].position = { m_Size.x,				m_CornerSize };
	m_Atlas[1][10].position = { m_Size.x,				m_Size.y - m_CornerSize };
	m_Atlas[1][11].position = { m_Size.x - m_CornerSize,m_Size.y - m_CornerSize };

	m_Atlas[2][0].position = { 0,						m_Size.y - m_CornerSize };
	m_Atlas[2][1].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Atlas[2][2].position = { m_CornerSize,			m_Size.y };
	m_Atlas[2][3].position = { 0,						m_Size.y };
	m_Atlas[2][4].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Atlas[2][5].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Atlas[2][6].position = { m_Size.x - m_CornerSize,	m_Size.y };
	m_Atlas[2][7].position = { m_CornerSize,			m_Size.y };
	m_Atlas[2][8].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Atlas[2][9].position = { m_Size.x,				m_Size.y - m_CornerSize };
	m_Atlas[2][10].position = { m_Size.x,				m_Size.y };
	m_Atlas[2][11].position = { m_Size.x - m_CornerSize,m_Size.y };
}

const sf::Vector2f& sfAtlasSprite::getSize() const
{
	return m_Size;
}

void sfAtlasSprite::setCornerSize(float size)
{
	m_CornerSize = size;
	setSize(m_Size);
}

void sfAtlasSprite::setAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR, const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR, const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR)
{
	setAtlasRect(0, 0, TL);
	setAtlasRect(0, 4, TC);
	setAtlasRect(0, 8, TR);

	setAtlasRect(1, 0, ML);
	setAtlasRect(1, 4, MC);
	setAtlasRect(1, 8, MR);

	setAtlasRect(2, 0, BL);
	setAtlasRect(2, 4, BC);
	setAtlasRect(2, 8, BR);
}

void sfAtlasSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	states.texture = m_Texture;
	target.draw(m_Atlas[0], states);
	target.draw(m_Atlas[1], states);
	target.draw(m_Atlas[2], states);
}

void sfAtlasSprite::setAtlasRect(int index, int start, const sf::FloatRect& rect)
{
	m_Atlas[index][0+start].texCoords = { rect.left,			rect.top };
	m_Atlas[index][1+start].texCoords = { rect.left + rect.width,	rect.top };
	m_Atlas[index][2+start].texCoords = { rect.left + rect.width,	rect.top + rect.height };
	m_Atlas[index][3+start].texCoords = { rect.left,			rect.top + rect.height };
}
