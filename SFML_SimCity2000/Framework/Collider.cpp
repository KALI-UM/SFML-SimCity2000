#include "pch.h"
#include "Collider.h"

Collider::Collider()
{
	Transform::Init(nullptr);
}

Collider::~Collider()
{
}

bool Collider::INITIALIZE()
{
	bool result = Initialize();
	return result;
}

void Collider::FIXEDUPDATE(float dt)
{
	m_Direction = Nomalize(getPosition() - m_PrevPosition);
	FixedUpdate(dt);
	m_PrevPosition = getPosition();
}

void Collider::OnCollisionEnter(Collider* collider)
{
	if (m_CollisionEnterFunc)
		m_CollisionEnterFunc(collider);
}

void Collider::OnCollisionStay(Collider* collider)
{
	if (m_CollisionStayFunc)
		m_CollisionStayFunc(collider);
}

void Collider::OnCollisionExit(Collider* collider)
{
	if (m_CollisionExitFunc)
		m_CollisionExitFunc(collider);
}

void Collider::OnTriggerEnter(Collider* collider)
{
	if (m_TriggerEnterFunc)
		m_TriggerEnterFunc(collider);
}

void Collider::OnTriggerStay(Collider* collider)
{
	if (m_TriggerStayFunc)
		m_TriggerStayFunc(collider);
}

void Collider::OnTriggerExit(Collider* collider)
{
	if (m_TriggerExitFunc)
		m_TriggerExitFunc(collider);
}
