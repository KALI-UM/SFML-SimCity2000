#pragma once

class DrawableObject;
class SceneBase;
class DebugInfo;
class GameObject : public Transform
{
public:
	const int	m_Id;
public:
	GameObject();
	GameObject(const GameObject& other);
	GameObject(GameObject&& other) noexcept;
	virtual ~GameObject();
	bool operator==(const GameObject& rhs);

	bool INITIALIZE();
	void RESET();
	void UPDATE(float dt);
	void LATEUPDATE(float dt);
	void FIXEDUPDATE(float dt);
	void IMGUIUPDATE();
	void RELEASE();

	void SetIsValid(bool value);
	bool GetIsValid()const;
	bool GetIsVisible()const;
	bool GetIsVisible(size_t index)const;

	DrawableObject* GetDrawable(size_t index = 0) const;
	DrawableObject* GetDrawable(const std::string& name) const;
	void SetDrawable(DrawableObject* dobj, bool isChild = true);
	int GetDrawbleCount()const { return (int)m_Drawable.size(); }

	//bool GetIsDrawSelf() const { return m_IsDrawSelf; }
	//void SetIsDrawSelf(bool drawSelf) { m_IsDrawSelf = drawSelf; }
	bool GetIsMovable() const { return m_IsMovable; }

	//void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	virtual bool Initialize();
	virtual void Reset();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixeUpdate(float dt);
	virtual void ImGuiUpdate();
	virtual void Release();

	std::vector<DrawableObject*> m_Drawable;
private:
	bool			m_IsValid;
	//bool			m_IsDrawSelf;
	const bool		m_IsMovable;

private:
	static int m_GameObjectsCount;
	static int m_IdNumber;
};

