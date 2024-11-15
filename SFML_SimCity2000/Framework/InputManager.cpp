#include "pch.h"
#include "InputManager.h"

bool InputManager::Initialize()
{
	Clear();
	m_HeldKey.reset();
	//m_HeldMouse.reset();

	m_Axis.resize(2);
	m_Axis[(int)Axis::Horizontal].AddKey(false, sf::Keyboard::A);
	m_Axis[(int)Axis::Horizontal].AddKey(true, sf::Keyboard::D);
	m_Axis[(int)Axis::Horizontal].AddKey(false, sf::Keyboard::Left);
	m_Axis[(int)Axis::Horizontal].AddKey(true, sf::Keyboard::Right);

	m_Axis[(int)Axis::Vertical].AddKey(false, sf::Keyboard::S);
	m_Axis[(int)Axis::Vertical].AddKey(true, sf::Keyboard::W);
	m_Axis[(int)Axis::Vertical].AddKey(false, sf::Keyboard::Down);
	m_Axis[(int)Axis::Vertical].AddKey(true, sf::Keyboard::Up);

	m_MouseDownPosition.resize((int)sf::Mouse::ButtonCount);

	return true;
}

void InputManager::UpdateEvent(const sf::Event& ev)
{

	switch (ev.type)
	{
	case sf::Event::KeyPressed:
	{
		if (!GetIsValidKey(ev.key.code))break;
		if (!GetKey(ev.key.code))
		{
			m_DownKey.set(ev.key.code, 1);
			m_HeldKey.set(ev.key.code, 1);
		}
		break;
	}
	case sf::Event::KeyReleased:
	{
		if (!GetIsValidKey(ev.key.code))break;
		m_UpKey.set(ev.key.code, 1);
		m_HeldKey.set(ev.key.code, 0);
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
		if (!GetIsValidMouse(ev.mouseButton.button))break;
		if (!GetMouse(ev.mouseButton.button))
		{
			m_MouseDownPosition[(int)ev.mouseButton.button] = GetMousePos();
			m_DownKey.set(sf::Keyboard::KeyCount + ev.mouseButton.button, 1);
			m_HeldKey.set(sf::Keyboard::KeyCount + ev.mouseButton.button, 1);
		}
		break;
	}
	case sf::Event::MouseButtonReleased:
	{
		if (!GetIsValidMouse(ev.mouseButton.button))break;
		m_UpKey.set(sf::Keyboard::KeyCount + ev.mouseButton.button, 1);
		m_HeldKey.set(sf::Keyboard::KeyCount + ev.mouseButton.button, 0);
		break;
	}
	break;
	}
}

void InputManager::Update(float dt)
{
	for (int axis = (int)Axis::Horizontal; axis <= (int)Axis::Vertical; axis++)
	{
		float raw = GetAxisRaw((Axis)axis);
		float dir = raw;
		auto& axisInfo = m_Axis[axis];
		if (dir == 0.0f && axisInfo.m_Value != 0.0f)
		{
			dir = axisInfo.m_Value > 0.0f ? -1.f : 1.f;
		}

		axisInfo.m_Value += dir * axisInfo.m_Sensitivity * dt;
		axisInfo.m_Value = Utils::Clamp(axisInfo.m_Value, -1.f, 1.f);

		//value가 완벽하게 0이 될 수 없지만 
		//현재 value값의 절대값이 이번 업데이트의 변위값보다 작다면 0을 지났다고 보고 0으로 보정해준다. 
		float stopThreshold = std::fabs(dir * axisInfo.m_Sensitivity * dt);
		if (raw == 0.0f && std::fabs(axisInfo.m_Value) < stopThreshold)
		{
			axisInfo.m_Value = 0.0f;
		}
	}
}

void InputManager::Clear()
{
	m_DownKey.reset();
	m_UpKey.reset();
	//m_DownMouse.reset();
	//m_UpMouse.reset();
}

bool InputManager::GetKeyDown(sf::Keyboard::Key key) const
{
	return m_DownKey.test(key);
}

bool InputManager::GetKeyUp(sf::Keyboard::Key key) const
{
	return m_UpKey.test(key);
}

bool InputManager::GetKey(sf::Keyboard::Key key) const
{
	return m_HeldKey.test(key);
}

sf::Vector2i InputManager::GetMousePos() const
{
	return sf::Mouse::getPosition(*GameManager::GetInstance()->GetWindow());
}

sf::Vector2f InputManager::GetMouseDefaultViewPos() const
{
	return GameManager::GetInstance()->GetWindow()->mapPixelToCoords(GetMousePos(), GameManager::GetInstance()->GetWindow()->getDefaultView());
}

sf::Vector2f InputManager::GetMouseViewPos(int index) const
{
	return GameManager::GetInstance()->GetWindow()->mapPixelToCoords(GetMousePos(), *GAME_MGR->GetView(index));
}

bool InputManager::GetMouseDown(sf::Mouse::Button btt) const
{
	return m_DownKey.test(sf::Keyboard::KeyCount + btt);
}

bool InputManager::GetMouseUp(sf::Mouse::Button btt) const
{
	return m_UpKey.test(sf::Keyboard::KeyCount + btt);
}

bool InputManager::GetMouse(sf::Mouse::Button btt) const
{
	return m_HeldKey.test(sf::Keyboard::KeyCount + btt);
}

bool InputManager::GetMouseDrag(sf::Mouse::Button btt) const
{
	return (!GetMouseDown(btt))&&GetMouse(btt);
}

sf::Vector2i InputManager::GetPrevMouseDown(sf::Mouse::Button btt) const
{
	return m_MouseDownPosition[btt];
}

float InputManager::GetAxis(Axis axis) const
{
	return m_Axis[(int)axis].m_Value;
}

float InputManager::GetAxisRaw(Axis axis) const
{
	float result = 0.0f;
	bool keyDown = false;

	auto& positiveKeys = m_Axis[(int)axis].m_PositiveKeys;
	auto& negativeKeys = m_Axis[(int)axis].m_NegativeKeys;
	for (int key = 0; key < sf::Keyboard::KeyCount + sf::Mouse::ButtonCount; key++)
	{
		if (!GetKeyKM(key)) continue;
		if (positiveKeys.find(key) != positiveKeys.end())
		{
			result += 1.0f;
		}
		if (negativeKeys.find(key) != negativeKeys.end())
		{
			result += -1.0f;
		}
		break;
	}
	return result;
}

bool InputManager::GetKeyDownKM(int key) const
{
	return m_DownKey.test(key);
}

bool InputManager::GetKeyUpKM(int key) const
{
	return m_UpKey.test(key);
}

bool InputManager::GetKeyKM(int key) const
{
	return m_HeldKey.test(key);
}

bool InputManager::GetIsValidKey(int key) const
{
	return key >= 0 && key <= sf::Keyboard::KeyCount;
}

bool InputManager::GetIsValidMouse(int key) const
{
	return key >= 0 && key <= sf::Mouse::ButtonCount;
}
