#pragma once

enum class Action;
class DAtlasSprite;
class SimCitySubButton;
class SimCityCursor;
class SimCityMenuBar :
    public GameObject
{
public:
	SimCityMenuBar(int viewIndex);
	~SimCityMenuBar();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void SetCursor(SimCityCursor* cursor) { m_Cursor = cursor; };
	bool GetHasFocus()const { return m_HasFocus; }

	void SetSubButton(const Action& action, const std::vector<std::pair<Action, std::string>>& sub);
protected:
	std::string m_BarTexId;

	DAtlasSprite* m_Bar;
	int m_ViewIndex = 0;

	std::vector <SimCitySubButton*> m_SubButtons;
	std::queue<SimCitySubButton*>	m_WantsToAddSubButtons;

private:
	SimCityCursor* m_Cursor;
	bool			m_HasFocus = false;
};

