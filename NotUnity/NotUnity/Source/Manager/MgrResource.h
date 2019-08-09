#ifndef MGR_RESOURCE_H
#define MGR_RESOURCE_H

#include <string>
#include <map>

#include "Manager.h"

class MgrResource : public Manager<MgrResource>
{
	friend Singleton<MgrResource>;
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
protected:
	MgrResource(std::string name = "MgrResource") : Manager<MgrResource>(name) {}
	~MgrResource() {}

	void LoadMeshes();
	void LoadMaterials();
};
#endif