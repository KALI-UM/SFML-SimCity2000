#pragma once

class DThickLine
	: public DrawableObject
{
public:
	DThickLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& line, float thick = 5.0f);
	DThickLine(sf::Vector2f points[], int size, const sf::Color& line, float thick = 5.0f);
	DThickLine(const DThickLine& other);
	DThickLine(DThickLine&& other);
	virtual ~DThickLine();

	void SetPoint(const sf::Vector2f& p1, const sf::Vector2f& p2);
	void SetPoints(sf::Vector2f points[], int size);
	void SetPoint(int index, const sf::Vector2f& p);
	void SetThickness(float thick);

	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;

	sf::Color GetColor() const;								//GetFillColor客 悼老		
	void SetColor(const sf::Color& color);					//SetFillColor客 悼老
	void SetColor(int r, int g, int b, int a = 255);		//SetFillColor客 悼老
	sf::Color GetFillColor() const;
	sf::Color GetOutlineColor() const;
	void SetFillColor(const sf::Color& color);
	void SetFillColor(int r, int g, int b, int a = 255);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineColor(int r, int g, int b, int a = 255);
	void SetOutlineThickness(float thick);
private:
	void SetRawToVertex(const sf::Vector2f& p1, const sf::Vector2f& p2);
	void SetRawToVertex(sf::Vector2f points[], int size);

private:
	sfLine	m_ThickLine;
	std::vector<sf::Vector2f> m_RawVertices;
	float	m_Thickness;
	float	m_HalfThickness;
};