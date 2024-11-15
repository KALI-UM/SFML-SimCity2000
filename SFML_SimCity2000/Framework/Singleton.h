#pragma once

template < typename T >
class Singleton
{
public:
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	static T* GetInstance()
	{
		if (m_Instance == nullptr)
			m_Instance = new T;
		return m_Instance;
	};
	static void DestroyInstance()
	{
		if (m_Instance) {
			delete m_Instance;
			m_Instance = nullptr;
		}
	};

private:
	static T* m_Instance;
};

template <typename T>
T* Singleton<T>::m_Instance = nullptr;