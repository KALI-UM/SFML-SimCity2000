#pragma once
#include "Singleton.h"
#include <bitset>

enum class Axis
{
	Horizontal = 0,
	Vertical = 1,
};

struct AxisInfo
{
	Axis m_Axis;

	std::set<int> m_PositiveKeys;
	std::set<int> m_NegativeKeys;

	float m_Sensitivity = 10;
	float m_Value = 0;

	void AddKey(bool positive, int code)
	{
		if (positive)
		{
			m_PositiveKeys.insert(code);
		}
		else
		{
			m_NegativeKeys.insert(code);
		}
	}
};

class InputManager :
	public Singleton<InputManager>
{
public:
	bool Initialize();
	void UpdateEvent(const sf::Event& ev);
	void Update(float dt);
	void Clear();

	bool GetKeyDown(sf::Keyboard::Key key) const;
	bool GetKeyUp(sf::Keyboard::Key key) const;
	bool GetKey(sf::Keyboard::Key key) const;

	sf::Vector2i GetMousePos()const;					//스크린 좌표계 기준
	sf::Vector2f GetMouseDefaultViewPos() const;		//기본뷰 좌표계 기준
	sf::Vector2f GetMouseViewPos(int index) const;

	bool GetMouseDown(sf::Mouse::Button btt) const;
	bool GetMouseUp(sf::Mouse::Button btt) const;
	bool GetMouse(sf::Mouse::Button btt) const;
	bool GetMouseDrag(sf::Mouse::Button btt)const;

	sf::Vector2i GetPrevMouseDown(sf::Mouse::Button btt) const;

	float GetAxis(Axis axis) const;
	float GetAxisRaw(Axis axis) const;
private:
	bool GetKeyDownKM(int key) const;
	bool GetKeyUpKM(int key) const;
	bool GetKeyKM(int key) const;

	bool GetIsValidKey(int key) const;
	bool GetIsValidMouse(int key) const;

	std::bitset<sf::Keyboard::KeyCount + sf::Mouse::ButtonCount>		m_HeldKey;
	std::bitset<sf::Keyboard::KeyCount + sf::Mouse::ButtonCount>		m_DownKey;
	std::bitset<sf::Keyboard::KeyCount + sf::Mouse::ButtonCount>		m_UpKey;
	//std::bitset<sf::Mouse::ButtonCount>		m_HeldMouse;
	//std::bitset<sf::Mouse::ButtonCount>		m_DownMouse;
	//std::bitset<sf::Mouse::ButtonCount>		m_UpMouse;
	std::vector<AxisInfo>							m_Axis;

	std::vector<sf::Vector2i>						m_MouseDownPosition;
};

#define MOUSEPOS (INPUT_MGR->GetMouseDefaultViewPos())