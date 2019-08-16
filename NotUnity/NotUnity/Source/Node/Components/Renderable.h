#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Graphics/Mesh.h"
#include "../../Utility/Graphics/Material.h"
#include "Transform.h"

enum class RENDER_PASS
{
	MANUAL,
	GEO,
	POST_FX,
	HUD,
	FINAL,
};

class MgrGraphics;
class Renderable : public Node, public TypeID<Renderable>, public Component
{
public:
	Renderable(std::string name = "Renderable");
	~Renderable();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();
	
	virtual void Render();
	 
	Renderable* AttachMesh(Mesh* mesh);
	Renderable* AttachMaterial(Material* material);
	Renderable* SelectShader(MgrGraphics::SHADER shader);

	RENDER_PASS GetRenderPass() const;
	Renderable* SetRenderPass(RENDER_PASS rp);
protected:
	Mesh* mesh;
	Material* material;
	Transform* t;
	MgrGraphics::SHADER shader;
	RENDER_PASS renderPass;
	

	void DrawMesh();
	void DrawMesh(unsigned count, unsigned offset);
};

#endif