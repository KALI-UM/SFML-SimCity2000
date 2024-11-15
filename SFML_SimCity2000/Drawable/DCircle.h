#pragma once

class DCircle :
	public DShape
{
public:
	DCircle(const sf::Vector2f& position, float radius, const sf::Color& line, float thick = 1.0f, const sf::Color& fill = sf::Color::Transparent, int pointcnt=20);
	DCircle(const sf::Vector2f& position, float radius, sf::Texture* tex, int pointcnt = 20);
	DCircle(const sf::Vector2f& position, float radius, const std::string& filepath, int pointcnt = 20);
	DCircle(const DCircle& other);
	DCircle(DCircle&& other);
	virtual ~DCircle();

	void SetRadius(float r);
	void SetPointCount(int cnt);
	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;
private:
	sf::CircleShape m_Circle;
};

