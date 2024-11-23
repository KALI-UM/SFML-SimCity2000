#include "pch.h"
#include "DTile.h"


sfTile::sfTile()
{
	m_Vertices.resize(5);
	m_Vertices.setPrimitiveType(sf::TriangleStrip);
}

sfTile::~sfTile()
{
}

void sfTile::setTexture(const sf::Texture& tex)
{
	m_Texture = &tex;
}

const sf::Texture* sfTile::getTexture() const
{
	return m_Texture;
}

void sfTile::setColor(const sf::Color& color)
{
	m_Color = color;
	for (int i = 0; i < m_Vertices.getVertexCount(); i++)
	{
		m_Vertices[i].color = color;
	}
}

const sf::Color& sfTile::getColor() const
{
	return m_Color;
}

sf::FloatRect sfTile::getGlobalBounds() const
{
	return getTransform().transformRect(m_Vertices.getBounds());
}

sf::FloatRect sfTile::getLocalBounds() const
{
	return m_Vertices.getBounds();
}

void sfTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	states.texture = m_Texture;
	target.draw(m_Vertices, states);
}

DTile::DTile(sf::Texture* tex)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
	}
}

DTile::DTile(const std::string& filepath)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	if (filepath != "")
	{
		sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
		if (tex)
		{
			m_IsValid = true;
			m_Tile.setTexture(*tex);
		}
	}
}

DTile::DTile(const sf::Vector2f& position, sf::Texture* tex)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	m_Tile.setPosition(position);
	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
	}
}

DTile::DTile(const sf::Vector2f& position, const std::string& filepath)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	m_Tile.setPosition(position);
	if (filepath != "")
	{
		sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
		if (tex)
		{
			m_IsValid = true;
			m_Tile.setTexture(*tex);
		}
	}
}

DTile::DTile(const DTile& other)
	:DrawableObject(other, &m_Tile, &m_Tile), m_Tile(other.m_Tile)
{
}

DTile::DTile(DTile&& other)
	:DrawableObject(other, &m_Tile, &m_Tile), m_Tile(other.m_Tile)
{
}

DTile::~DTile()
{
}

void DTile::SetShapeLot(const TileShapeType& shape, const sf::Vector2u& lot)
{
	//if (m_ShapeType == shape && m_Tile.m_LotSize == lot)return;
	m_ShapeType = shape;
	m_Tile.m_LotSize = lot;

	auto lot_float = m_Tile.m_LotSize.To<float>();
	lot_float.y = (float)GetTextureSize().y / m_Unit;
	switch (m_ShapeType)
	{
	case TileShapeType::Convex:
	{
		m_Tile.m_Vertices.resize(5);
		m_Tile.m_Vertices[0].position = { 0,0 };
		m_Tile.m_Vertices[1].position = { 0 , lot_float.y - lot_float.x * 0.25f };
		m_Tile.m_Vertices[2].position = { lot_float.x ,0 };
		m_Tile.m_Vertices[3].position = { lot_float.x * 0.5f , lot_float.y };
		m_Tile.m_Vertices[4].position = { lot_float.x ,lot_float.y - lot_float.x * 0.25f };
	}
	break;
	case TileShapeType::Diamond:
	{
		m_Tile.m_Vertices.resize(4);
		m_Tile.m_Vertices[0].position = { 0 , lot_float.y - lot_float.x * 0.25f };
		m_Tile.m_Vertices[1].position = { lot_float.x * 0.5f , lot_float.y };
		m_Tile.m_Vertices[2].position = { lot_float.x * 0.5f,lot_float.y * 0.5f };
		m_Tile.m_Vertices[3].position = { lot_float.x , lot_float.y - lot_float.x * 0.25f };
	}
	break;
	case TileShapeType::Rectangle:
		m_Tile.m_Vertices.resize(4);
		m_Tile.m_Vertices[0].position = { 0 , 0 };
		m_Tile.m_Vertices[1].position = { lot_float.x ,0 };
		m_Tile.m_Vertices[2].position = { lot_float.x ,lot_float.y };
		m_Tile.m_Vertices[3].position = { 0, lot_float.y };
		break;
	default:
		break;
	}

	for (int i = 0; i < m_Tile.m_Vertices.getVertexCount(); i++)
	{
		m_Tile.m_Vertices[i].position *= m_Unit;
		m_Tile.m_Vertices[i].texCoords = m_Tile.m_Vertices[i].position += sf::Vector2f(-1, -1);
	}
}

void DTile::SetTexture(sf::Texture* tex, TileShapeType type, const sf::Vector2u& lot)
{
	if (lot == sf::Vector2u(0, 0) || !tex)
	{
		m_IsValid = false;
		return;
	}

	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
		SetShapeLot(type, lot);
	}
}

void DTile::SetTexture(const std::string& filepath, TileShapeType type, const sf::Vector2u& lot)
{
	if (filepath == "")
		SetTexture(nullptr, type, lot);
	else
		SetTexture(ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath), type, lot);
}

sf::Vector2u DTile::GetTextureSize() const
{
	return m_Tile.getTexture() ? m_Tile.getTexture()->getSize() : sf::Vector2u(0, 0);
}

void DTile::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	if (m_Tile.getTexture())
		setOrigin(((GetTextureSize().x / 2) * ((int)type % 3)) + detail.x,
			((GetTextureSize().y / 2) * ((int)type / 3)) + detail.y);
	else
		DrawableObject::SetOrigin(type, detail);
}

sf::FloatRect DTile::GetGlobalBounds() const
{
	return m_Tile.getGlobalBounds();
}

sf::FloatRect DTile::GetLocalBounds() const
{
	return m_Tile.getLocalBounds();
}

sf::Color DTile::GetColor() const
{
	return m_Tile.getColor();
}

void DTile::SetColor(const sf::Color& color)
{
	SetFillColor(color);
}

void DTile::SetColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

sf::Color DTile::GetFillColor() const
{
	return m_Tile.getColor();
}

sf::Color DTile::GetOutlineColor() const
{
	return sf::Color();
}

void DTile::SetFillColor(const sf::Color& color)
{
	m_Tile.setColor(color);
}

void DTile::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DTile::SetOutlineColor(const sf::Color& color)
{
	//¹ÌÁö¿ø
}

void DTile::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}
