#ifndef MANAGER_H
#define MANAGER_H

#include "../Utility/Singleton.h"
#include "../Node/Node.h"
#include "../Utility/TypeID.h"

template <typename T>
class Manager : public Singleton<T>, public Node, public TypeID<T>
{		
public:
	virtual void Start() = 0;
	virtual void Update(double dt) = 0;
	virtual void End() = 0;
protected:
	Manager<T>(std::string name = "Manager") : Node(name) {}
	~Manager<T>() {}
};
#endif