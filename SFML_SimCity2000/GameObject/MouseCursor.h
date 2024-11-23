#pragma once
#include "GameObject.h"
class DSprite;
class MouseCursor :
	public GameObject
{
public:
	MouseCursor(const std::string& texId = "ui/push.png", int viewIndex=0);
	~MouseCursor();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void SetMouseDownFunc(sf::Mouse::Button btt, const std::function<void()>& func);
	void SetMouseUpFunc(sf::Mouse::Button btt, const std::function<void()>& func);
	void SetMouseFunc(sf::Mouse::Button btt, const std::function<void()>& func);

	void SetCursorViewIndex(int index) { m_ViewIndex = index; }

protected:
	std::string m_TextureId;
	DSprite* m_CursorSprite;

	int m_ViewIndex = 0;
	std::vector<std::function<void()>> m_WhenDownFunc;
	std::vector<std::function<void()>> m_WhenUpFunc;
	std::vector<std::function<void()>> m_WhenFunc;

};

