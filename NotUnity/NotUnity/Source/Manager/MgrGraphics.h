#ifndef MGR_GRAPHICS_H
#define MGR_GRAPHICS_H

#include <string>
#include <map>

#include "Manager.h"
#include "../Utility/Graphics/Mesh.h"

class Scene;
class MgrGraphics : public Manager<MgrGraphics>
{
	friend Singleton<MgrGraphics>;	
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	void CacheMesh(Mesh* mesh);
	Mesh* GetCachedMesh(std::string name);
protected:
	MgrGraphics(std::string name = "MgrGraphics") : Manager<MgrGraphics>(name) {}
	~MgrGraphics() {}

	std::map <std::string, GLuint> shaderPrograms;
	std::map<std::string, Mesh*> cachedMeshes;
};
#endif