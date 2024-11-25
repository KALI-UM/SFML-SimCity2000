#pragma once
#include "sfAtlasSprite.h"

class DAtlasSprite :
    public DrawableObject
{
public:
	DAtlasSprite(sf::Texture* tex);
	DAtlasSprite(const std::string& filepath);
	DAtlasSprite();
	DAtlasSprite(const DAtlasSprite& other);
	DAtlasSprite(DAtlasSprite&& other);
	virtual ~DAtlasSprite();

	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;

	void SetTexture(sf::Texture* tex);
	void SetTexture(const std::string& filepath);
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

	void SetSize(const sf::Vector2f& size);
	void SetCornerSize(float size);
	void SetAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR,
		const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR,
		const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR);

private:
	sfAtlasSprite m_Sprite;
};

