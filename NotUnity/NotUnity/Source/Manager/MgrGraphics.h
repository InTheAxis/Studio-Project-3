#ifndef MGR_GRAPHICS_H
#define MGR_GRAPHICS_H

#include <string>
#include <map>

#include "Manager.h"
#include "../Utility/Graphics/Mesh.h"
#include "../Utility/Resource.h"

class Scene;
class MgrGraphics : public Manager<MgrGraphics>
{
	friend Singleton<MgrGraphics>;	
	friend Node;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	

	enum SHADER
	{
		DEFAULT = 0,
		NUM_SHADERS
	};

	void UseShader(MgrGraphics::SHADER shader);

	void CacheMesh(Mesh* mesh);
	Mesh* GetCachedMesh(std::string name);
protected:
	MgrGraphics(std::string name = "MgrGraphics") : Manager<MgrGraphics>(name) {}
	~MgrGraphics() {}

	SHADER currShader;

	std::map <int, GLuint> shaderPrograms;
	std::map<std::string, Mesh*> cachedMeshes;
};
#endif