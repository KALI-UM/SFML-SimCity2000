#pragma once

class DAnimation :
	public DrawableObject
{
public:
	DAnimation(sf::Texture* tex, const sf::Vector2u& FrameCnt, float speed = 1.0f, bool playnow = true, bool loop = true);
	DAnimation(const std::string& filepath, const sf::Vector2u& FrameCnt, float speed = 1.0f, bool playnow = true, bool loop = true);
	DAnimation(const sf::Vector2f& position, sf::Texture* tex, const sf::Vector2u& FrameCnt, float speed = 1.0f, bool playnow = true, bool loop = true);
	DAnimation(const sf::Vector2f& position, const std::string& filepath, const sf::Vector2u& FrameCnt, float speed = 1.0f, bool playnow = true, bool loop = true);
	DAnimation(const DAnimation& other);
	DAnimation(DAnimation&& other);
	virtual ~DAnimation();

	void Update(float dt) override;
	void SetTexture(sf::Texture* tex);

	virtual void SetOrigin(OriginType type, const sf::Vector2f& detail = sf::Vector2f(0, 0));
	sf::Color GetColor() const;
	virtual void SetColor(const sf::Color& color);
	virtual void SetColor(int r, int g, int b, int a = 255);
	virtual sf::Color GetFillColor() const;								//GetColor客 悼老						
	virtual sf::Color GetOutlineColor() const;							//GetColor客 悼老
	virtual void SetFillColor(const sf::Color& color);					//SetColor客 悼老
	virtual void SetFillColor(int r, int g, int b, int a = 255);		//SetColor客 悼老
	virtual void SetOutlineColor(const sf::Color& color);				//SetColor客 悼老
	virtual void SetOutlineColor(int r, int g, int b, int a = 255);		//SetColor客 悼老
	
	sf::FloatRect GetGlobalBounds()const;
	sf::FloatRect GetLocalBounds()const;

	sf::Vector2u GetTextureSize()const;

	sf::Vector2u GetFrame() const;
	void SetFrame(unsigned int x, unsigned int y);
	void SetFrame(const sf::Vector2u& v);
	bool GetIsLoopAnim()const;
	void SetIsLoopAnim(bool playnow);

private:
	sf::Sprite m_Sprite;
	sf::Vector2u m_FrameCount;
	sf::Vector2u m_FrameSize;
	sf::Vector2u m_CurrFrame;

	float m_Speed;
	float m_FrameTime = 0;
	bool m_IsPlaying;			//犁积吝牢瘤
	bool m_IsLoopAnim;
};

