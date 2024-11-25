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
	//bool operator==(const GameObject& rhs);

	bool INITIALIZE();
	void RESET();
	void UPDATE(float dt);
	void LATEUPDATE(float dt);
	void FIXEDUPDATE(float dt);
	void PRERENDER();
	void POSTRENDER();
	void IMGUIUPDATE();
	void RELEASE();

	void SetIsValid(bool valid) { m_IsValid = valid; };
	bool GetIsValid()const { return m_IsValid; }
	void SetIsVisible(bool visible) { m_IsVisible = visible; }
	bool GetIsVisible()const;
	bool GetIsVisible(size_t index)const;
	bool GetIsChildObj()const { return m_ParentObj != nullptr; }
	void SetParentObj(GameObject* parent, bool isTransformParent = true);
	void SetChildObj(GameObject* child, bool isTransformChild = true);
	void RemoveChildObj(GameObject* child);

	DrawableObject* GetDrawableObj(size_t index = 0) const;
	DrawableObject* GetDrawableObj(const std::string& name) const;
	void SetDrawable(DrawableObject* dobj, bool isChild = true);
	int GetDrawbleCount()const { return (int)m_Drawables.size(); }

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
	virtual void PreRender();
	virtual void PostRender();
	virtual void ImGuiUpdate();
	virtual void Release();

	std::vector<DrawableObject*> m_Drawables;
	GameObject* m_ParentObj=nullptr;
	std::list<GameObject*>		m_ChildrenObjs;
private:
	bool			m_IsValid;
	//bool			m_IsDrawSelf;
	bool			m_IsVisible = true;
	const bool		m_IsMovable;

private:
	static int m_GameObjectsCount;
	static int m_IdNumber;
};

