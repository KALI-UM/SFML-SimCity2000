#include "pch.h"
#include "GameObject.h"

int GameObject::m_GameObjectsCount = 1;
int GameObject::m_IdNumber = 0;

GameObject::GameObject()
	:m_Id(m_IdNumber++), m_IsValid(true), m_IsMovable(true)
{
	Transform::Init(nullptr);
	m_GameObjectsCount++;
}

GameObject::GameObject(const GameObject& other)
	:m_Id(m_IdNumber++), m_IsValid(other.m_IsValid), m_IsMovable(other.m_IsMovable)
{
	Transform::Init(other, nullptr);
	m_GameObjectsCount++;
}

GameObject::GameObject(GameObject&& other)noexcept
	:m_Id(other.m_Id), m_IsValid(other.m_IsValid), m_Drawables(other.m_Drawables), m_IsMovable(other.m_IsMovable)
{
	//¹Ì¿Ï
	Transform::Init(other, nullptr);
	other.m_Drawables.clear();
	other.SetIsValid(false);
}

GameObject::~GameObject()
{
	m_GameObjectsCount--;
}

bool GameObject::INITIALIZE()
{
	bool result = Initialize();
	for (auto& child : m_ChildrenObjs)
	{
		result &= child->INITIALIZE();
	}
	return result;
}

void GameObject::RESET()
{
	Reset();
}

void GameObject::UPDATE(float dt)
{
	Update(dt);

	for (auto& drawable : m_Drawables)
	{
		drawable->Update(dt);
	}

	for (auto& child : m_ChildrenObjs)
	{
		if (child->GetIsValid())
		{
			child->UPDATE(dt);
		}
	}
}

void GameObject::LATEUPDATE(float dt)
{
	LateUpdate(dt);

	for (auto& child : m_ChildrenObjs)
	{
		child->LATEUPDATE(dt);
	}
}

void GameObject::FIXEDUPDATE(float dt)
{
	FixeUpdate(dt);

	for (auto& child : m_ChildrenObjs)
	{
		child->FIXEDUPDATE(dt);
	}
}

void GameObject::PRERENDER()
{
	PreRender();

	for (auto& child : m_ChildrenObjs)
	{
		child->PRERENDER();
	}
}

void GameObject::POSTRENDER()
{
	PostRender();

	for (auto& child : m_ChildrenObjs)
	{
		child->POSTRENDER();
	}
}

void GameObject::IMGUIUPDATE()
{
	ImGuiUpdate();

	for (auto& drawable : m_Drawables)
	{
		drawable->ImGuiUpdate();
	}
}

void GameObject::RELEASE()
{
	Release();

	for (auto& drawable : m_Drawables)
	{
		delete drawable;
	}
}

//void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
//
//	for (auto& dobj : m_Drawable)
//		target.draw(*dobj->GetDrawable(), states);
//}

bool GameObject::Initialize()
{
	return true;
}

void GameObject::Reset()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::LateUpdate(float dt)
{
}

void GameObject::FixeUpdate(float dt)
{
}

void GameObject::PreRender()
{
}

void GameObject::PostRender()
{
}

void GameObject::ImGuiUpdate()
{
}

void GameObject::Release()
{
}

bool GameObject::GetIsVisible() const
{
	return m_IsVisible && GetIsValid() && GetDrawbleCount() != 0;
}

bool GameObject::GetIsVisible(size_t index) const
{
	//return GetIsValid() && GetDrawable(index) && GetDrawable(index)->GetIsVisible();
	return m_IsValid && m_IsVisible && GetDrawableObj(index) && GetDrawableObj(index)->GetIsValid();
}

void GameObject::SetParentObj(GameObject* parent, bool isTransformParent)
{
	if (m_ParentObj)
	{
		m_ParentObj->RemoveChildObj(this);
	}
	m_ParentObj = parent;
	if (m_ParentObj)
		m_ParentObj->SetChildObj(this, isTransformParent);
}

void GameObject::SetChildObj(GameObject* child, bool isTransformChild)
{
	child->m_ParentObj = this;
	m_ChildrenObjs.push_back(child);
	if (isTransformChild)
		this->Transform::SetChild(child);
}

void GameObject::RemoveChildObj(GameObject* child)
{
	for (auto gobj = m_ChildrenObjs.begin(); gobj != m_ChildrenObjs.end(); gobj++)
	{
		if (*gobj == child)
		{
			child->m_ParentObj = nullptr;
			m_ChildrenObjs.erase(gobj);
			Transform::RemoveChild(child);
			break;
		}
	}
}

DrawableObject* GameObject::GetDrawableObj(size_t index)const
{
	if (index >= m_Drawables.size())return nullptr;
	return m_Drawables[index];
}

DrawableObject* GameObject::GetDrawableObj(const std::string& name) const
{
	for (auto& drawable : m_Drawables)
	{
		if (drawable->GetName() == name)
			return drawable;
	}
	return nullptr;
}

void GameObject::SetDrawable(DrawableObject* dobj, bool isChild)
{
	if (isChild)
		this->Transform::SetChild(dobj);
	m_Drawables.push_back(dobj);
}

