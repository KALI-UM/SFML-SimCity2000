#pragma once
#include "MouseCursor.h"

enum class CursorMode
{
	None,
	Move,
	WaitForClick,
	NowDrag,
};

class SimCityCursor :
	public MouseCursor
{
public:
	SimCityCursor(const std::string& texId = "ui/push.png", int viewIndex = 0);
	~SimCityCursor();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;

	void SetCursorMode(CursorMode mode);

protected:
	CursorMode m_CurrentMode;
	std::string m_ModeTextureId[3];
};

