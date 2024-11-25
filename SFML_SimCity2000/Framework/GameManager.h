#pragma once

namespace sf
{
	class RenderWindow;
}

//class ResourceManager<sf::Texture>;
//class ResourceManager<sf::Font>;

enum class GameMode
{
	Debug,
	Cheat,
	Normal
};

class InputManager;
class SceneManager;
class DrawableObject;
class DebugInfo;

struct PriorityComp
{
	bool operator()(DrawableObject*& lhs, DrawableObject*& rhs);
};

class GameManager :
	public Singleton<GameManager>
{
public:
	GameManager();

	bool Initialize(sf::RenderWindow* window);
	void Release();

	void UpdateEvent(const sf::Event& ev);
	void Update(float dt);
	void Render();
	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void ImGuiUpdate();

	sf::RenderWindow* GetWindow();
	sf::View* GetView(int index);
	const sf::FloatRect& GetViewRect(int index);

	void ResizeViews(unsigned int cnt);
	void SetViewNeedPriority(int index, bool need);
	int GetViewCount()const;

	sf::Vector2f GetScreenToViewPos(int index, const sf::Vector2i& screenPos);
	sf::Vector2i GetViewToScreenPos(int index, const sf::Vector2f& viewPos);

	void SetViewSize(int index, const sf::FloatRect& rect);
	void SetViewZoom(int index, float zoom);
	void SetViewportSize(int index, const sf::FloatRect& rect);
	void SetViewCenter(int index, const sf::Vector2f& pos);
	void MoveView(int index, const sf::Vector2f& offset);
	void SetViewRotation(int index, float rot);
	void RotateView(int index, float rot);
	void UpdateViewRect();

	void PushDrawableObject_PQ(int viewindex, DrawableObject* dobj);
	void PushDrawableObject_Q(int viewindex, DrawableObject* dobj);
	void PushDebugDrawObject(int viewindex, DebugInfo* dobj);

	const GameMode& GetGameMode()const;


private:
	sf::RenderWindow*						m_MainWindow;

	GameMode								m_GameMode;


	struct ViewDrawInfo
	{
		sf::View							view;
		sf::FloatRect						viewRect;
		bool								needPriority;
		std::priority_queue<DrawableObject*, std::vector<DrawableObject*>, PriorityComp> drawQue_PQ;
		std::queue<DrawableObject*> drawQue_Q;
	};
	std::vector<ViewDrawInfo>				m_Views;

	struct ViewDebugInfo
	{
		sf::View							view;
		std::queue<DebugInfo*>				drawQue;
	};
	std::vector<ViewDebugInfo>				m_DebugViews;
};

