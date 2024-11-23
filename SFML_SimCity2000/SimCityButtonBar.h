#pragma once


enum class ButtonName;
class DAtlasSprite;
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
	void SetButtonFunc(std::function<void(const ButtonName&)> func) { m_ButtonFunc = func; };
protected:
	std::string m_BarTexId;
	std::string m_ButtonTexId;

	DAtlasSprite* m_Bar;
	std::vector<DSprite*> m_Buttons;
	std::function<void(const ButtonName&)> m_ButtonFunc;
	std::vector<bool> m_ButtonWork;
	int m_ViewIndex = 0;

	int m_CurrButtonIndex = 0;

private:
	SimCityCursor* m_Cursor;
};

