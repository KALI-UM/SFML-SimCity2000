#pragma once

class DSprite :
	public DrawableObject
{
public:
	DSprite(sf::Texture* tex);
	DSprite(const std::string& filepath);
	DSprite(const sf::Vector2f& position, sf::Texture* tex);
	DSprite(const sf::Vector2f& position, const std::string& filepath);
	DSprite();
	DSprite(const DSprite& other);
	DSprite(DSprite&& other);
	virtual ~DSprite();

	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;

	void SetTexture(sf::Texture* tex, bool resetRect = false);
	void SetTexture(const std::string& filepath, bool resetRect = false);
	void SetTextureRect(const sf::IntRect& rect);
	void SetOrigin(OriginType type, const sf::Vector2f& detail = sf::Vector2f(0, 0));
	sf::Vector2u GetTextureSize() const;
	sf::Color GetColor() const;
	void SetColor(const sf::Color& color);
	void SetColor(int r, int g, int b, int a = 255);
	sf::Color GetFillColor() const;								//GetColor客 悼老						
	sf::Color GetOutlineColor() const;							//GetColor客 悼老
	void SetFillColor(const sf::Color& color);					//SetColor客 悼老
	void SetFillColor(int r, int g, int b, int a = 255);		//SetColor客 悼老
	void SetOutlineColor(const sf::Color& color);				//SetColor客 悼老
	void SetOutlineColor(int r, int g, int b, int a = 255);		//SetColor客 悼老

private:
	sf::Sprite m_Sprite;
};

