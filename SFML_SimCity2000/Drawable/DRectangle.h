#pragma once

class DRectangle :
	public DShape
{
public:
	DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, const sf::Color& line, float thick = 1.0f, const sf::Color& fill = sf::Color::Transparent);
	DRectangle(const sf::FloatRect& rect, const sf::Color& line, float thick = 1.0f, const sf::Color& fill = sf::Color::Transparent);
	DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, sf::Texture* tex);
	DRectangle(const sf::FloatRect& rect, sf::Texture* tex);
	DRectangle(const sf::Vector2f& position, const sf::Vector2f& widthheight, const std::string& filepath);
	DRectangle(const sf::FloatRect& rect, const std::string& filepath);
	DRectangle(const DRectangle& other);
	DRectangle(DRectangle&& other);
	virtual ~DRectangle();

	void SetSize(const sf::Vector2f& widthheight);
	void SetFloatRect(const sf::FloatRect& frect);
	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;
private:
	sf::RectangleShape m_Rectangle;
	sf::FloatRect m_FloatRect;
};

