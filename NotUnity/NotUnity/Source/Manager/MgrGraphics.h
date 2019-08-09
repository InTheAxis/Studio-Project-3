#ifndef MGR_GRAPHICS_H
#define MGR_GRAPHICS_H

#include <string>
#include <map>

#include "Manager.h"
#include "../Utility/Graphics/Mesh.h"
#include "../Utility/Resource.h"
#include "../Utility/Math/Mtx44.h"
#include "../Utility/Math/Vector3.h"
#include "../Utility/Math/Vector4.h"

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
		CURRENT = -1,
		DEFAULT = 0,
		SIMPLE,
		NUM_SHADERS
	};

	void UseShader(MgrGraphics::SHADER shader);

	void CacheMesh(Mesh* mesh);
	Mesh* GetCachedMesh(std::string name);

	void SetUniform(std::string uniform, int i, MgrGraphics::SHADER shader = CURRENT);
	void SetUniform(std::string uniform, bool b, MgrGraphics::SHADER shader = CURRENT);
	void SetUniform(std::string uniform, const Mtx44& m, MgrGraphics::SHADER shader = CURRENT);
	void SetUniform(std::string uniform, const Vector3& v, MgrGraphics::SHADER shader = CURRENT);
	void SetUniform(std::string uniform, const Vector4& v, MgrGraphics::SHADER shader = CURRENT);
	void SetUniform(std::string uniform, float f, MgrGraphics::SHADER shader = CURRENT);
protected:
	MgrGraphics(std::string name = "MgrGraphics") : Manager<MgrGraphics>(name) {}
	~MgrGraphics() {}

	SHADER currShader;

	std::map <int, GLuint> shaderPrograms;
	std::map<std::string, Mesh*> cachedMeshes;
	std::map<std::string, unsigned> cachedUniforms;

	unsigned GetUniLoc(std::string uniform, MgrGraphics::SHADER shader);
};
#endif