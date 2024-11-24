#include "pch.h"
#include "DrawableObject.h"
#include "DebugInfo.h"

DrawableObject::DrawableObject(DataType datatype, sf::Drawable* drawable, sf::Transformable* transform, bool debug)
	:m_DataType(datatype), m_Drawable(drawable), m_Transform(transform), m_IsValid(false), m_DebugInfo(nullptr)
{
	Init(m_Transform);
#ifdef _DEBUG
	if (debug)
		m_DebugInfo = new DebugInfo(sf::FloatRect(), getTransform(), const_cast<sf::Vector2f&>(m_Transform->getPosition()));
#endif // _DEBUG
}

DrawableObject::DrawableObject(const DrawableObject& other, sf::Drawable* drawable, sf::Transformable* transform)
	:m_DataType(other.m_DataType), m_Drawable(drawable), m_Transform(transform), m_IsValid(other.m_IsValid), m_DebugInfo(nullptr)
{
	Init(other, m_Transform);
#ifdef _DEBUG
	if (other.m_DebugInfo)
		m_DebugInfo = new DebugInfo(sf::FloatRect(), getTransform(), const_cast<sf::Vector2f&>(m_Transform->getPosition()));
#endif // _DEBUG
}

DrawableObject::DrawableObject(DrawableObject&& other, sf::Drawable* drawable, sf::Transformable* transform)
	:m_DataType(other.m_DataType), m_Drawable(drawable), m_Transform(transform), m_IsValid(other.m_IsValid), m_DebugInfo(nullptr)
{
	Init(other, m_Transform);
#ifdef _DEBUG
	if (other.m_DebugInfo)
		m_DebugInfo = new DebugInfo(sf::FloatRect(), getTransform(), const_cast<sf::Vector2f&>(m_Transform->getPosition()));
#endif // _DEBUG
	other.m_IsValid = false;
}

DrawableObject::~DrawableObject()
{
	delete m_DebugInfo;
}

void DrawableObject::SetPriorityType(DrawPriorityType type, float value)
{
	m_PriorityType = type;
	m_PriorityValue = value;
}

float DrawableObject::GetPriorityValue() const
{
	switch (m_PriorityType)
	{
	case DrawPriorityType::X:
		return getPosition().x;
		break;
	case DrawPriorityType::Y:
		return getPosition().y;
		break;
	case DrawPriorityType::Custom:
		return m_PriorityValue;
		break;
	default:
		return 0;
		break;
	}
}

DebugInfo* DrawableObject::GetDebugDraw()
{
	if (m_DebugInfo)
		m_DebugInfo->Update(GetLocalBounds(), getTransform());
	return m_DebugInfo;
}

void DrawableObject::SetDebugDraw(bool debug)
{
	if (!m_DebugInfo && debug)
	{
		m_DebugInfo = new DebugInfo(GetLocalBounds(), getTransform(), const_cast<sf::Vector2f&>(m_Transform->getPosition()));
	}
	else if (m_DebugInfo && !debug)
	{
		delete m_DebugInfo;
		m_DebugInfo = nullptr;
	}
}

void DrawableObject::SetOriginCenter()
{
	SetOrigin(OriginType::MC);
}

void DrawableObject::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	setOrigin(((GetGlobalBounds().width / 2) * ((int)type % 3)) + detail.x,
		((GetGlobalBounds().height / 2) * ((int)type / 3)) + detail.y);
}

sf::Vector2f DrawableObject::GetBoundPointPosition(int index) const
{
	//0 , 1
	//3 , 2 ¿Œµ¶Ω∫
	if (index == 2)index = 3;
	else if (index == 3) index = 2;

	return sf::Vector2f(GetGlobalBounds().getPosition().x + (index % 2) * GetGlobalBounds().width,
		GetGlobalBounds().getPosition().y + (index / 2) * GetGlobalBounds().height);
}

sf::Vector2u DrawableObject::GetTextureSize() const
{
	return sf::Vector2u(0, 0);
}
