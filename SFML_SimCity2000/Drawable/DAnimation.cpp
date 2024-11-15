#include "pch.h"
#include "DAnimation.h"

DAnimation::DAnimation(sf::Texture* tex, const sf::Vector2u& FrameCnt, float speed, bool playnow, bool loop)
	:DrawableObject(DataType::Animation, &m_Sprite, &m_Sprite),
	m_FrameCount(FrameCnt), m_FrameSize({ tex->getSize().x / FrameCnt.x, tex->getSize().y / FrameCnt.y }), m_CurrFrame({ 0,0 }),
	m_Speed(speed), m_IsPlaying(playnow), m_IsLoopAnim(loop)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
		m_Sprite.setTextureRect(sf::IntRect(0, 0, m_FrameSize.x, m_FrameSize.y));
	}
}

DAnimation::DAnimation(const std::string& filepath, const sf::Vector2u& FrameCnt, float speed, bool playnow, bool loop)
	:DrawableObject(DataType::Animation, &m_Sprite, &m_Sprite),
	m_FrameCount(FrameCnt), m_FrameSize(), m_CurrFrame({ 0,0 }),
	m_Speed(speed), m_IsPlaying(playnow), m_IsLoopAnim(loop)
{
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
		m_FrameSize = { tex->getSize().x / m_FrameCount.x, tex->getSize().y / m_FrameCount.y };
		m_Sprite.setTextureRect(sf::IntRect(0, 0, m_FrameSize.x, m_FrameSize.y));
	}
}

DAnimation::DAnimation(const sf::Vector2f& position, sf::Texture* tex, const sf::Vector2u& FrameCnt, float speed, bool playnow, bool loop)
	:DrawableObject(DataType::Animation,&m_Sprite, &m_Sprite),
	m_FrameCount(FrameCnt), m_FrameSize({ tex->getSize().x / FrameCnt.x, tex->getSize().y / FrameCnt.y }), m_CurrFrame({ 0,0 }),
	m_Speed(speed), m_IsPlaying(playnow), m_IsLoopAnim(loop)
{
	m_Sprite.setPosition(position);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
		m_Sprite.setTextureRect(sf::IntRect(0, 0, m_FrameSize.x, m_FrameSize.y));
	}
}

DAnimation::DAnimation(const sf::Vector2f& position, const std::string& filepath, const sf::Vector2u& FrameCnt, float speed, bool playnow, bool loop)
	:DrawableObject(DataType::Animation, &m_Sprite, &m_Sprite),
	m_FrameCount(FrameCnt), m_FrameSize(), m_CurrFrame({ 0,0 }),
	m_Speed(speed), m_IsPlaying(playnow), m_IsLoopAnim(loop)
{
	m_Sprite.setPosition(position);
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
		m_FrameSize = { tex->getSize().x / m_FrameCount.x, tex->getSize().y / m_FrameCount.y };
	}
}

DAnimation::DAnimation(const DAnimation& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite),
	m_FrameCount(other.m_FrameCount), m_FrameSize(other.m_FrameSize), m_CurrFrame(other.m_CurrFrame),
	m_Speed(other.m_Speed), m_IsPlaying(other.m_IsPlaying), m_IsLoopAnim(other.m_IsLoopAnim)
{
}

DAnimation::DAnimation(DAnimation&& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite),
	m_FrameCount(other.m_FrameCount), m_FrameSize(other.m_FrameSize), m_CurrFrame(other.m_CurrFrame),
	m_Speed(other.m_Speed), m_IsPlaying(other.m_IsPlaying), m_IsLoopAnim(other.m_IsLoopAnim)
{
}

DAnimation::~DAnimation()
{
}

void DAnimation::Update(float dt)
{
	m_FrameTime += dt * m_Speed;
	if (m_FrameTime > 1.0f)
	{
		m_FrameTime = 0;
		m_CurrFrame.x++;
		if (m_IsLoopAnim)
			m_CurrFrame.x %= m_FrameCount.x;
		else if (m_FrameCount.x >= m_FrameCount.x)
			m_CurrFrame.x = m_FrameCount.x - 1;


		m_Sprite.setTextureRect(
			sf::IntRect(m_CurrFrame.x * m_FrameSize.x, m_CurrFrame.y * m_FrameSize.y,
				m_FrameSize.x, m_FrameSize.y));
	}
}

void DAnimation::SetTexture(sf::Texture* tex)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
		m_FrameSize = { tex->getSize().x / m_FrameCount.x, tex->getSize().y / m_FrameCount.y };
	}
}

void DAnimation::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	setOrigin(((GetTextureSize().x / 2) * ((int)type % 3)) + detail.x,
		((GetTextureSize().y / 2) * ((int)type / 3)) + detail.y);
}

sf::Color DAnimation::GetColor() const
{
	return m_Sprite.getColor();
}

void DAnimation::SetColor(const sf::Color& color)
{
	m_Sprite.setColor(color);
}

void DAnimation::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color DAnimation::GetFillColor() const
{
	return GetColor();
}

sf::Color DAnimation::GetOutlineColor() const
{
	return GetColor();
}

void DAnimation::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void DAnimation::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DAnimation::SetOutlineColor(const sf::Color& color)
{
	SetColor(color);
}

void DAnimation::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}

sf::FloatRect DAnimation::GetGlobalBounds() const
{
	return m_Sprite.getGlobalBounds();
}

sf::FloatRect DAnimation::GetLocalBounds() const
{
	return m_Sprite.getLocalBounds();
}

sf::Vector2u DAnimation::GetTextureSize() const
{
	return m_Sprite.getTexture() ? m_Sprite.getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::Vector2u DAnimation::GetFrame() const
{
	return m_CurrFrame;
}

void DAnimation::SetFrame(unsigned int x, unsigned int y)
{
	SetFrame({ x,y });
}

void DAnimation::SetFrame(const sf::Vector2u& v)
{
	m_CurrFrame = v;
}

bool DAnimation::GetIsLoopAnim() const
{
	return m_IsPlaying;
}

void DAnimation::SetIsLoopAnim(bool playnow)
{
	m_IsPlaying = playnow;
}

