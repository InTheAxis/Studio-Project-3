#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>

class Node
{
public:
	Node() {}
	~Node() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	bool IsActive();
	void ActiveSelf(bool active);
	double GetTimeAlive();

	template <typename T>
	T* AddChild(std::string key = std::to_string(T::m_id), T* ptr = nullptr)
	{
		if (m_children.count(key) > 0)
			return nullptr;

		T* child = ptr ? ptr : new T;
		m_children[key] = static_cast<Node*>(child);
		
		return GetChild<T>(key);
	}

	template <typename T>
	T* GetChild(std::string key = std::to_string(T::m_id))
	{
		if (m_children.count(key) <= 0)
			return nullptr;
		return static_cast<T*>(m_children[key]);
	}
protected:
	bool m_active;
	std::map<std::string, Node*> m_children;

	double m_lifetime;
};

#endif