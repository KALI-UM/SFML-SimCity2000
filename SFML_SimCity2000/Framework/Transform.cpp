#include "pch.h"
#include "Transform.h"

//Transform::Transform(sf::Transformable* transformable)
//	//:m_T(transformable),
//	//m_ParentPosition({ 0,0 }), m_LocalPosition(m_T->getPosition()),
//	//m_ParentRotation(0), m_LocalRotation(m_T->getRotation()),
//	//m_ParentScale({ 1,1 }), m_LocalScale(m_T->getScale())
//{
//}
//
//Transform::Transform(const Transform& other, sf::Transformable* transformable)
//	//:m_T(transformable),
//	//m_ParentPosition(other.getParentPosition()), m_LocalPosition(other.getLocalPosition()),
//	//m_ParentRotation(other.getParentRotation()), m_LocalRotation(other.getLocalRotation()),
//	//m_ParentScale(other.getParentScale()), m_LocalScale(other.getLocalScale())
//{
//}
//
//Transform::Transform(Transform&& other, sf::Transformable* transformable)
//	//:m_T(transformable),
//	//m_ParentPosition(other.getParentPosition()), m_LocalPosition(other.getLocalPosition()),
//	//m_ParentRotation(other.getParentRotation()), m_LocalRotation(other.getLocalRotation()),
//	//m_ParentScale(other.getParentScale()), m_LocalScale(other.getLocalScale())
//{
//}

void Transform::Init(sf::Transformable* transformable)
{
	m_T = transformable;
	if (!m_T)m_T = new sf::Transformable();
	Reset();
	m_LocalScale = { 1,1 };
}

void Transform::Init(const Transform& other, sf::Transformable* transformable)
{
	m_T = transformable;
	if (!m_T)m_T = new sf::Transformable();
	//m_ParentPosition = other.getParentPosition();
	m_LocalPosition = other.getLocalPosition();
	//m_ParentRotation = other.getParentRotation();
	m_LocalRotation = other.getLocalRotation();
	//m_ParentScale = other.getParentScale();
	m_LocalScale = other.getLocalScale();
}

void Transform::Reset()
{
	ResetLocalTransform();
}

void Transform::ResetLocalTransform()
{
	m_LocalPosition = m_T->getPosition();
	m_LocalRotation = m_T->getRotation();
	m_LocalScale = m_T->getScale();
}

void Transform::SetChild(Transform* child)
{
	child->m_Parent = this;
	m_Children.push_back(child);
	child->setPosition();
	child->setRotation();
	child->setScale();
}

void Transform::RemoveChild(Transform* child)
{
	for (auto t = m_Children.begin(); t != m_Children.end(); t++)
	{
		if (*t == child)
		{
			child->m_Parent = nullptr;
			child->setPosition();
			child->setRotation();
			child->setScale();
			m_Children.erase(t);
			break;
		}
	}
}

void Transform::SetParent(Transform* parent)
{
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;
	if (m_Parent)
		m_Parent->SetChild(this);
}

void Transform::setPosition(const sf::Vector2f& pos)
{
	m_LocalPosition = pos - getParentPosition();
	setPosition();
}

void Transform::setPosition(float posx, float posy)
{
	setPosition(sf::Vector2f(posx, posy));
}

void Transform::setLocalPosition(const sf::Vector2f& localpos)
{
	m_LocalPosition = localpos;
	setPosition();
}

void Transform::move(const sf::Vector2f& pos)
{
	m_LocalPosition += pos;
	m_T->move(pos);
	for (auto& child : m_Children)
	{
		child->setPosition();
	}
}

void Transform::setPosition()
{
	//m_T->setPosition(getParentPosition() + sf::Vector2f(m_LocalPosition.x * getParentScale().x, m_LocalPosition.y * getParentScale().y));
	m_T->setPosition(getParentPosition() + m_LocalPosition);
	for (auto& child : m_Children)
	{
		child->setPosition();
	}
}

void Transform::setRotation(float rot)
{
	m_LocalRotation = rot - getParentRotation();
	setRotation();
}

void Transform::setLocalRotation(float localrot)
{
	m_LocalRotation = localrot;
	setRotation();
}

void Transform::rotate(float rot)
{
	m_LocalRotation += rot;
	m_T->rotate(rot);
	for (auto& child : m_Children)
	{
		child->setRotation();
	}
}

void Transform::setRotation()
{
	m_T->setRotation(getParentRotation() + m_LocalRotation);
	for (auto& child : m_Children)
	{
		child->setRotation();
	}
}

void Transform::setScale(const sf::Vector2f& scale)
{
	m_LocalScale = { scale.x / getParentScale().x, scale.y / getParentScale().y };
	setScale();
}

void Transform::setScale(float scalex, float scaley)
{
	setScale(sf::Vector2f(scalex, scaley));
}

void Transform::setLocalScale(const sf::Vector2f& localscale)
{
	m_LocalScale = localscale;
	setScale();
}

void Transform::setScale()
{
	m_T->setScale({ getParentScale().x * m_LocalScale.x, getParentScale().y * m_LocalScale.y });
	for (auto& child : m_Children)
	{
		child->setScale();
	}
}

void Transform::setOrigin(const sf::Vector2f& origin)
{
	m_T->setOrigin(origin);
}

void Transform::setOrigin(float originx, float originy)
{
	setOrigin(sf::Vector2f(originx, originy));
}

sf::Vector2f Transform::getParentPosition() const
{
	return (m_Parent ?
		m_Parent->getPosition() : sf::Vector2f(0, 0));
}

float Transform::getParentRotation() const
{
	return (m_Parent ?
		m_Parent->getRotation() : 0);
}

sf::Vector2f Transform::getParentScale() const
{
	return (m_Parent ?
		m_Parent->getScale() : sf::Vector2f(1, 1));
}
