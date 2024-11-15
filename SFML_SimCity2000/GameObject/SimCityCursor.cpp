#include "pch.h"
#include "SimCityCursor.h"

SimCityCursor::SimCityCursor(const std::string& texId, int viewIndex)
{
}

SimCityCursor::~SimCityCursor()
{
}

bool SimCityCursor::Initialize()
{
	bool result = true;
	m_ModeTextureId[0] = "ui/push.png";
	m_ModeTextureId[1] = "ui/push.png";
	m_ModeTextureId[2] = "ui/push.png";
	SetCursorMode(CursorMode::None);

	result&=MouseCursor::Initialize();
	return result;
}

void SimCityCursor::Reset()
{

	MouseCursor::Reset();
}

void SimCityCursor::Update(float dt)
{
}

void SimCityCursor::SetCursorMode(CursorMode mode)
{
	if (m_CurrentMode == mode)return;
	m_CurrentMode = mode;
	m_TextureId = m_ModeTextureId[(int)mode];
	m_CursorSprite->SetTexture(m_TextureId, true);
}
