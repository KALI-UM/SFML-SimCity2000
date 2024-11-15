#include "pch.h"
#include "EffectObject.h"

bool EffectObject::IsNeedToReturn()
{
	return m_Duration <= m_PlayTime;
}

EffectObject::EffectObject(int cnt, DSprite* particle)
	:m_Duration(1), m_PlayTime(0), m_Speed(1), m_IsUsingDT(false)
{
	m_Particles.resize(cnt);
	m_Particles[0] = particle;
	for (int i = 1; i < cnt; i++)
	{
		m_Particles[i] = new DSprite(*particle);
	}
}

EffectObject::EffectObject(int cnt, DRectangle* particle)
	:m_Duration(1), m_PlayTime(0), m_Speed(1), m_IsUsingDT(false)

{
	m_Particles.resize(cnt);
	m_Particles[0] = particle;
	for (int i = 1; i < cnt; i++)
	{
		m_Particles[i] = new DRectangle(*particle);
	}
}

EffectObject::EffectObject(int cnt, DCircle* particle)
	:m_Duration(1), m_PlayTime(0), m_Speed(1), m_IsUsingDT(false)
{
	m_Particles.resize(cnt);
	m_Particles[0] = particle;
	for (int i = 1; i < cnt; i++)
	{
		m_Particles[i] = new DCircle(*particle);
	}
}

EffectObject::EffectObject(int cnt, DLine* particle)
	:m_Duration(1), m_PlayTime(0), m_Speed(1), m_IsUsingDT(false)
{
	m_Particles.resize(cnt);
	m_Particles[0] = particle;
	for (int i = 1; i < cnt; i++)
	{
		m_Particles[i] = new DLine(*particle);
	}
}

EffectObject::~EffectObject()
{
}

bool EffectObject::Initialize()
{
	for (auto& drawable : m_Particles)
	{
		SetDrawable(drawable);
	}

	return true;
}

void EffectObject::Reset()
{
	SetIsValid(false);
	m_PlayTime = 0;
}

void EffectObject::Update(float dt)
{
	if (!m_IsUsingDT)
		dt = FRAMEWORK->GetRealDeltaTime();
	m_PlayTime += dt;
	EffectPlay(dt);
}

void EffectObject::Effect(float duration, const sf::Vector2f& point, float speed)
{
	SetIsValid(true);
	m_Duration = duration;
	m_Position = point;
	m_Speed = speed;
	SetPosition();
}

void EffectObject::SetUsingDeltaTime(bool dt)
{
	m_IsUsingDT = dt;
}
