#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include "../Utility/WinDebug.h"

//defines what a "thing" is this engine
//inherit from this and typeID for full functionality
//basically a node can store other nodes as children, and can be started, updated and ended
//please name your nodes starting with N
//call the base node start,update and end functions in your deerived class node
/********************************************
  EXAMPLE USAGE

class NExample : public Node, public TypeID<NExample>
{
	NExample(std::string name = "NExample") : Node(name) {}
	~NExample() {}

	virtual void Start();
	{
		Node::Start();
	}
	virtual void Update(double dt)
	{
		Node::Update(dt);
	}
	virtual void End()
	{
		Node::End();
	}
};
********************************************/

class Node
{
public:	
	Node(std::string name = "node") : m_name(name) { m_active = true; parent = nullptr; }
	~Node() {}
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	void SetName(std::string name);
	std::string GetName();
	bool IsActive();
	void ActiveSelf(bool active);
	double GetTimeAlive();

	template <typename T>
	T* AddChild(std::string key = std::to_string(T::m_id), T* ptr = nullptr)
	{
		if (m_children.count(key) > 0)
			return nullptr;

		T* child = ptr ? ptr : new T(key);
		child->parent = this;
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
	std::string m_name;
	bool m_active;
	std::map<std::string, Node*> m_children;

	double m_lifetime;
	Node* parent;
};

#endif