#pragma once

class sfAtlasSprite
	:public sf::Drawable, public sf::Transformable
{
	public:
		sfAtlasSprite();
		~sfAtlasSprite() {};
		sf::VertexArray m_Vertices;
		sf::VertexArray m_Atlas[3];

		sf::Color m_Color;
		const sf::Texture* m_Texture;
		sf::Vector2f m_Size;
		float m_CornerSize;

		void setTexture(const sf::Texture& tex);
		const sf::Texture* getTexture() const;
		void setColor(const sf::Color& color);
		const sf::Color& getColor()const;
		sf::FloatRect getGlobalBounds()const;
		sf::FloatRect getLocalBounds()const;
		void setSize(const sf::Vector2f& size);	//코너 사이즈보다 커야한다.
		const sf::Vector2f& getSize() const;
		void setCornerSize(float size);
		void setAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR, const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR, const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR);
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setAtlasRect(int index, int start, const sf::FloatRect& rect);
};

