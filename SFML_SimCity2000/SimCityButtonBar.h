#pragma once

enum class Action;
class DAtlasSprite;
class SimCitySubButton;
class SimCityCursor;
class SimCityButtonBar
	:public GameObject
{
public:
	SimCityButtonBar(int viewIndex);
	~SimCityButtonBar();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void SetCursor(SimCityCursor* cursor) { m_Cursor = cursor; };
	void SetButtonFunc(std::function<void(const Action&)> func) { m_ButtonFunc = func; };
	bool GetHasFocus()const { return m_HasFocus; }

	void SetSubButton(const Action& action, const std::vector<std::pair<Action, std::string>>& sub);
protected:
	std::string m_BarTexId;
	std::string m_ButtonTexId;

	DAtlasSprite* m_Bar;
	std::vector<DSprite*> m_Buttons;
	std::function<void(const Action&)> m_ButtonFunc;
	std::vector<bool> m_ButtonWork;


	int m_ViewIndex = 0;
	int m_ButtonCount = 18;
	int m_CurrButtonIndex = 0;

	std::vector <SimCitySubButton*> m_SubButtons;
	std::queue<SimCitySubButton*>	m_WantsToAddSubButtons;

private:
	SimCityCursor* m_Cursor;
	bool			m_HasFocus = false;
};

