#pragma once
#include "MouseCursor.h"

enum class ButtonName;
class SimCityCursor :
	public MouseCursor
{
public:
	SimCityCursor(const std::string& texId = "ui/cursor.png", int viewIndex = 0);
	~SimCityCursor();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;

	void SetCursorMode(ButtonName mode);
	ButtonName GetCursorMode()const { return m_CurrentMode; }

protected:
	ButtonName m_CurrentMode;
	sf::IntRect m_ModeTextureRect[25];
};

