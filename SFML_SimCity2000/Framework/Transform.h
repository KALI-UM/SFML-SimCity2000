#pragma once

//sf::Transformable을 계층용으로 사용하기 위해 제작

namespace sf
{
	class Transformable;
}

class Transform
{
protected:
	void Init(sf::Transformable* transformable);
	void Init(const Transform& other, sf::Transformable* transformable);
public:
	void Reset();
	void ResetLocalTransform();
	void SetChild(Transform* child);
	void RemoveChild(Transform* child);
	void SetParent(Transform* parent);

	const sf::Transform& getTransform() const { return m_T->getTransform(); }
	const sf::Transform& getInverseTransform() const { return m_T->getInverseTransform(); }


	void setPosition(const sf::Vector2f& pos);					//global
	void setPosition(float posx, float posy);					//global
	void setLocalPosition(const sf::Vector2f& localpos);
	void move(const sf::Vector2f& pos);
private:
	void setPosition();

public:
	void setRotation(float rot);								//global
	void setLocalRotation(float localrot);
	void rotate(float rot);
private:
	void setRotation();

public:
	void setScale(const sf::Vector2f& scale);					//global
	void setScale(float scalex, float scaley);					//global
	void setLocalScale(const sf::Vector2f& localscale);
private:
	void setScale();

public:
	void setOrigin(const sf::Vector2f& origin);
	void setOrigin(float originx, float originy);

	sf::Vector2f getPosition()const { return m_T->getPosition(); }
	sf::Vector2f getParentPosition()const;
	sf::Vector2f getLocalPosition()const { return m_LocalPosition; }

	float getRotation()const { return m_T->getRotation(); }
	float getParentRotation()const;
	float getLocalRotation()const { return m_LocalRotation; }

	sf::Vector2f getScale()const { return m_T->getScale(); }
	sf::Vector2f getParentScale()const;
	sf::Vector2f getLocalScale()const { return m_LocalScale; }

	sf::Vector2f getOrigin()const { return m_T->getOrigin(); }

private:
	sf::Vector2f	m_LocalPosition;
	float			m_LocalRotation;
	sf::Vector2f	m_LocalScale;

	sf::Transformable* m_T;

	Transform* m_Parent = nullptr;
	std::vector<Transform*>	m_Children;
};

