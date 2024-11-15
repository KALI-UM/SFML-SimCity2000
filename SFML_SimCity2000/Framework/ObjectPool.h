#pragma once

//ObjectPoolable을 상속받은 오브젝트만 가능
enum class ExpandOption
{
	MakeNew,
	GetOldUsed,
	Nullptr,
};

template<typename T>
class ObjectPool
{
private:
	std::list<T*>	unused;
	std::list<T*>	used;
	std::queue<T*>	needToReturn;

	ExpandOption	m_Expand;						//take해줄 수 없을때 오브젝트를 생성할 것인지
	int				m_LayerIndex;
	SceneBase*		m_Scene;
public:
	ObjectPool()
	{
	}

	void Initialize(SceneBase* scene, int count, ExpandOption autoExpand, int layerIndex)
	{
		m_Scene = scene;
		m_Expand = autoExpand;
		m_LayerIndex = layerIndex;
		for (int i = 0; i < count; ++i)
		{
			auto obj = new T();
			obj->InitForObjectPool();
			unused.push_back(obj);
		}
	}

	void Reset()
	{
		while (!used.empty())
		{
			auto& obj = used.front();
			Return(obj);
		}
	}

	void Update(float dt)
	{
		for (auto obj : used)
		{
			if (obj->IsNeedToReturn())
				needToReturn.push(obj);
		}

		while (!needToReturn.empty())
		{
			auto obj = needToReturn.front();
			Return(obj);
			needToReturn.pop();
		}
	}

	void Exit()
	{
		for (auto obj : unused)
		{
			delete obj;
		}
	}

	~ObjectPool()
	{
	}

	T* Take()
	{
		if (unused.empty())
		{
			switch (m_Expand)
			{
			case ExpandOption::MakeNew:
			{
				auto obj = new T();
				obj->InitForObjectPool();
				obj->SetIsValid(true);
				used.push_back(obj);
				m_Scene->AddGameObject(m_LayerIndex, obj);
				return obj;
			}
				break;
			case ExpandOption::GetOldUsed:
			{
				auto obj = used.front();
				used.pop_front();
				obj->InitForObjectPool();
				obj->SetIsValid(true);
				used.push_back(obj);
				return obj;
			}
				break;
			case ExpandOption::Nullptr:
			{
				return nullptr;
			}
				break;
			}		
		}

		auto obj = unused.front();
		unused.pop_front();
		used.push_back(obj);
		obj->SetIsValid(true);
		m_Scene->AddGameObject(m_LayerIndex, obj);
		return obj;
	}

	void Return(T* obj)
	{
		auto find = std::find(used.begin(), used.end(), obj);
		if (find == used.end())
		{
			std::cout << "Invaild Obj" << std::endl;
			return;
		}
		used.erase(find);
		obj->InitForObjectPool();
		obj->SetIsValid(false);
		unused.push_back(obj);
		m_Scene->RemoveGameObject(m_LayerIndex, obj);
	}
};