#include "pch.h"
#include "DThickLine.h"

DThickLine::DThickLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& line, float thick)
	:DrawableObject(DataType::Shape, &m_ThickLine, &m_ThickLine)
	, m_Thickness(thick), m_HalfThickness(thick / 2)
{
	m_ThickLine.m_Color = line;
	SetPoint(point1, point2);
	m_IsValid = true;
}

DThickLine::DThickLine(sf::Vector2f points[], int size, const sf::Color& line, float thick)
	:DrawableObject(DataType::Shape, &m_ThickLine, &m_ThickLine)
{
	m_ThickLine.m_Color = line;
	SetPoints(points, size);
	m_IsValid = true;
}

DThickLine::DThickLine(const DThickLine& other)
	:DrawableObject(other, &m_ThickLine, &m_ThickLine)
	, m_Thickness(other.m_Thickness), m_HalfThickness(other.m_HalfThickness)
{
}

DThickLine::DThickLine(DThickLine&& other)
	:DrawableObject(other, &m_ThickLine, &m_ThickLine)
	, m_Thickness(other.m_Thickness), m_HalfThickness(other.m_HalfThickness)
{
}

DThickLine::~DThickLine()
{
}

void DThickLine::SetPoint(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	m_ThickLine.m_Vertices.setPrimitiveType(sf::TriangleStrip);
	m_ThickLine.m_Vertices.resize(4);

	//p1과 p2를 잇는 선분에 직교하는 선분의 기울기를 역탄젠트해 직교하는 선분을 빗변으로 하는 삼각형의 세타값을 구한다. 
	//코사인세타, 사인세타값으로 p1을 중심으로 하고 길이가 thick인 포인트의 위치를 구할 수 있다.
	float theta = atan2f(-p2.x + p1.x, p2.y - p1.y);
	m_ThickLine.m_Vertices[0].position = sf::Vector2f(m_RawVertices[0].x + m_HalfThickness * cosf(theta), m_RawVertices[0].y + m_HalfThickness * sinf(theta));
	m_ThickLine.m_Vertices[1].position = sf::Vector2f(m_RawVertices[0].x - m_HalfThickness * cosf(theta), m_RawVertices[0].y - m_HalfThickness * sinf(theta));
	m_ThickLine.m_Vertices[2].position = sf::Vector2f(m_RawVertices[1].x + m_HalfThickness * cosf(theta), m_RawVertices[1].y + m_HalfThickness * sinf(theta));
	m_ThickLine.m_Vertices[3].position = sf::Vector2f(m_RawVertices[1].x - m_HalfThickness * cosf(theta), m_RawVertices[1].y - m_HalfThickness * sinf(theta));
	SetColor(m_ThickLine.m_Color);
}

void DThickLine::SetPoints(sf::Vector2f points[], int size)
{
	m_ThickLine.m_Vertices.setPrimitiveType(sf::TriangleStrip);
	m_ThickLine.m_Vertices.resize(4 * (size - 1));
	float theta = 0;
	for (int i = 1; i < size; i++)
	{
		int j = i - 1;
		theta = atan2f(-points[i].x + points[j].x, points[i].y - points[j].y);
		m_ThickLine.m_Vertices[j * 4].position = sf::Vector2f(points[j].x + m_HalfThickness * cosf(theta), points[j].y + m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[j * 4 + 1].position = sf::Vector2f(points[j].x - m_HalfThickness * cosf(theta), points[j].y - m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[j * 4 + 2].position = sf::Vector2f(points[i].x + m_HalfThickness * cosf(theta), points[i].y + m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[j * 4 + 3].position = sf::Vector2f(points[i].x - m_HalfThickness * cosf(theta), points[i].y - m_HalfThickness * sinf(theta));
	}
	SetColor(m_ThickLine.m_Color);
}

void DThickLine::SetPoint(int index, const sf::Vector2f& p)
{
	if (m_RawVertices.size() <= index)
	{
		return;
	}
	m_RawVertices[index] = p;

	int i;
	if (index == 0)i = 0;
	else if (index == m_RawVertices.size() - 1)
	{
		index -= 1;
		i = index;
	}
	else i = index - 1;

	for (; i <= index; i++)
	{
		float theta = atan2f(-m_RawVertices[i + 1].x + m_RawVertices[i].x, m_RawVertices[i + 1].y - m_RawVertices[i].y);
		m_ThickLine.m_Vertices[i * 4].position = sf::Vector2f(m_RawVertices[i].x + m_HalfThickness * cosf(theta), m_RawVertices[i].y + m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[i * 4 + 1].position = sf::Vector2f(m_RawVertices[i].x - m_HalfThickness * cosf(theta), m_RawVertices[i].y - m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[i * 4 + 2].position = sf::Vector2f(m_RawVertices[i + 1].x + m_HalfThickness * cosf(theta), m_RawVertices[i + 1].y + m_HalfThickness * sinf(theta));
		m_ThickLine.m_Vertices[i * 4 + 3].position = sf::Vector2f(m_RawVertices[i + 1].x - m_HalfThickness * cosf(theta), m_RawVertices[i + 1].y - m_HalfThickness * sinf(theta));
	}
}

void DThickLine::SetThickness(float thick)
{
	if (m_Thickness == thick)return;

	m_Thickness = thick;
	m_HalfThickness = m_Thickness / 2;
	for (int i = 0; i < m_RawVertices.size(); i++)
	{
		SetPoint(i, m_RawVertices[i]);
	}
}

sf::FloatRect DThickLine::GetGlobalBounds() const
{
	return m_ThickLine.getGlobalBounds();
}

sf::FloatRect DThickLine::GetLocalBounds() const
{
	return m_ThickLine.getLocalBounds();
}

sf::Color DThickLine::GetColor() const
{
	return m_ThickLine.getColor();
}

void DThickLine::SetColor(const sf::Color& color)
{
	m_ThickLine.setColor(color);
}

void DThickLine::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color DThickLine::GetFillColor() const
{
	return GetColor();
}

sf::Color DThickLine::GetOutlineColor() const
{
	return GetColor();
}

void DThickLine::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void DThickLine::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DThickLine::SetOutlineColor(const sf::Color& color)
{
	SetColor(color);
}

void DThickLine::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}

void DThickLine::SetOutlineThickness(float thick)
{
	SetThickness(thick);
}

void DThickLine::SetRawToVertex(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	m_RawVertices.resize(2);
	m_RawVertices[0] = p1;
	m_RawVertices[1] = p2;
}

void DThickLine::SetRawToVertex(sf::Vector2f points[], int size)
{
	m_RawVertices.resize(size);
	for (int i = 0; i < size; i++)
	{
		m_RawVertices[i] = points[i];
	}
}
