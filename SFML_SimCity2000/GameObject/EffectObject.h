#pragma once
#include "GameObject.h"
#include "ObjectPoolable.h"

class DSprite;
class DRectangle;
class DCircle;
class DLine;

class EffectObject :
	public GameObject, public ObjectPoolable
{
public:
	bool IsNeedToReturn();
	void InitForObjectPool() = 0;

protected:
	EffectObject(int cnt, DSprite* particle);
	EffectObject(int cnt, DRectangle* particle);
	EffectObject(int cnt, DCircle* particle);
	EffectObject(int cnt, DLine* particle);
	~EffectObject();
public:
	bool Initialize();
	void Reset();
	void Update(float dt);

	void Effect(float duration, const sf::Vector2f& point, float speed = 1.0f);

	void SetUsingDeltaTime(bool dt);

protected:
	virtual void EffectPlay(float dt) = 0;
	virtual void SetPosition() = 0;
	std::vector<DrawableObject*> m_Particles;
	float				m_Duration;			//몇초동안 재생될 effect인가 
	float				m_PlayTime;			//타이머
	float				m_Speed;
	sf::Vector2f		m_Position = { 0,0 };			//재생 위치

	bool m_IsUsingDT;
};

