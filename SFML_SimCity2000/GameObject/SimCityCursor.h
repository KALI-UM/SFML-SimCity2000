#pragma once
#include "MouseCursor.h"

enum class Action;
class SimCityCursor :
	public MouseCursor
{
public:
	SimCityCursor(const std::string& texId = "ui/cursor.png", int viewIndex = 0);
	~SimCityCursor();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;

	void SetCursorMode(Action mode);
	Action GetCursorMode()const { return m_CurrentMode; }

protected:
	Action m_CurrentMode;
	sf::IntRect m_ModeTextureRect[25];
};

