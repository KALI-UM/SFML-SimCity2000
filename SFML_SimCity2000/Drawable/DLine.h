#pragma once

class DLine :
	public DrawableObject
{
public:
	DLine(const sf::Vector2f& point1, const sf::Vector2f& point2, const sf::Color& line);
	DLine(sf::Vector2f points[], int size, const sf::Color& line, bool isStrip = true);
	DLine(const DLine& other);
	DLine(DLine&& other);
	virtual ~DLine();

	void SetPoint(const sf::Vector2f& p1, const sf::Vector2f& p2);
	void SetPoints(sf::Vector2f points[], int size, bool isStrip = true);
	void SetPoint(int index, const sf::Vector2f& p);
	sf::Vector2f GetPoint(int index) const;
	int GetPointsSize() const;

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

private:
	sfLine	m_Line;
};

