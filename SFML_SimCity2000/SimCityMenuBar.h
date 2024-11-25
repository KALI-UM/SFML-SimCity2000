#pragma once

enum class Menu;
struct MenuSet;
class DAtlasSprite;
class DText;
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

	void SetMenuString(const Menu& menu, const std::string& name);
	void SetSubButton(const Menu& menu, const std::vector<std::pair<Menu, MenuSet&>>& sub);
protected:
	std::string m_BarTexId;
	DAtlasSprite* m_Bar;
	std::string m_FontId;
	std::vector<DText*> m_Buttons;

	int m_ViewIndex = 0;
	int m_ButtonCount =2;
	int m_CurrButtonIndex = 0;

	std::vector<SimCitySubButton*>	m_SubButtons;
	std::queue<SimCitySubButton*>	m_WantsToAddSubButtons;

private:
	SimCityCursor*	m_Cursor;
	bool			m_HasFocus = false;
};

