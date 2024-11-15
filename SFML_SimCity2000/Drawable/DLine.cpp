#include "pch.h"
#include "DLine.h"

DLine::DLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& line)
	:DrawableObject(DataType::Shape, &m_Line, &m_Line)
{
	m_Line.m_Color = line;
	SetPoint(point1, point2);
	m_IsValid = true;
}

DLine::DLine(sf::Vector2f points[], int size, const sf::Color& line, bool isStrip)
	:DrawableObject(DataType::Shape, &m_Line, &m_Line)
{
	m_Line.m_Color = line;
	SetPoints(points, size, isStrip);
	m_IsValid = true;
}

DLine::DLine(const DLine& other)
	:DrawableObject(other, &m_Line, &m_Line)
{
}

DLine::DLine(DLine&& other)
	:DrawableObject(other, &m_Line, &m_Line)
{
}

DLine::~DLine()
{
}

void DLine::SetPoint(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	m_Line.m_Vertices.setPrimitiveType(sf::LineStrip);
	m_Line.m_Vertices.resize(2);
	m_Line.m_Vertices[0].position = p1;
	m_Line.m_Vertices[1].position = p2;
	SetColor(m_Line.m_Color);
}

void DLine::SetPoints(sf::Vector2f points[], int size, bool isStrip)
{
	if (isStrip)
		m_Line.m_Vertices.setPrimitiveType(sf::LineStrip);
	else
		m_Line.m_Vertices.setPrimitiveType(sf::Lines);

	m_Line.m_Vertices.resize(size);
	for (int i = 0; i < size; i++)
	{
		m_Line.m_Vertices[i].position = points[i];
	}
	SetColor(m_Line.m_Color);
}

sf::Vector2f DLine::GetPoint(int index) const
{
	if (index >= m_Line.m_Vertices.getVertexCount())
		return sf::Vector2f(0, 0);
	return m_Line.m_Vertices[index].position;
}

int DLine::GetPointsSize() const
{
	return m_Line.m_Vertices.getVertexCount();
}

void DLine::SetPoint(int index, const sf::Vector2f& p)
{
	if (m_Line.m_Vertices.getVertexCount() <= index)
	{
		return;
	}
	m_Line.m_Vertices[index].position = p;
}

sf::FloatRect DLine::GetGlobalBounds() const
{
	return m_Line.getGlobalBounds();
}

sf::FloatRect DLine::GetLocalBounds() const
{
	return m_Line.getLocalBounds();
}

sf::Color DLine::GetColor() const
{
	return m_Line.getColor();
}

void DLine::SetColor(const sf::Color& color)
{
	m_Line.setColor(color);
}

void DLine::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color DLine::GetFillColor() const
{
	return GetColor();
}

sf::Color DLine::GetOutlineColor() const
{
	return GetColor();
}

void DLine::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void DLine::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DLine::SetOutlineColor(const sf::Color& color)
{
	SetColor(color);
}

void DLine::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}

