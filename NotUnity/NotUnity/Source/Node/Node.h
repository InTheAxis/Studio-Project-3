#ifndef NODE_H
#define NODE_H

#include <string>
#include <map>
#include "../Utility/WinDebug.h"

//defines what a "thing" is this engine
//inherit from this and typeID for full functionality
//basically a node can store other nodes as children, and can be started, updated and ended
//call the base node start,update and end functions in your deerived class node
//use constructor for creation, like adding children/default values
//use Start() for init for refrences and maybe values
//optional funcs like onEnable to use in derived
/********************************************
  EXAMPLE USAGE

class Example : public Node, public TypeID<NExample>
{
	Example(std::string name = "NExample") : Node(name) 
	{
	}
	~Example() 
	{
	}

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
	virtual void OnEnable()
	{
		//subscribe to delegates here
	}
	virtual void OnDisable()
	{
		//unsubscribe to delegates here
	}
};
********************************************/

class Node
{
public:	
	Node(std::string name = "node");
	virtual ~Node();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	virtual void OnEnable();
	virtual void OnDisable();

	void SetName(std::string name);
	std::string GetName();
	bool IsActive();
	void ActiveSelf(bool active);
	double GetTimeAlive();
	Node* GetParent();

	template <typename T>
	T* AddChild(std::string key = std::to_string(T::m_id), T* ptr = nullptr)
	{
		if (m_children.count(key) > 0)
			return GetChild<T>(key);

		if (!ptr)
		{
			ptr = new T(key);
			m_newed = true;
		}
		T* child = ptr;
		child->parent = this;
		child->m_active = m_active;
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

	bool m_newed;
};

#endif