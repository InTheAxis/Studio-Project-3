#include "ColliderRender.h"
#include "../Manager/MgrGraphics.h"
#include "../../Utility/Math/MyMath.h"
#include "../../Utility/Graphics/MeshBuilder.h"

ColliderRender::ColliderRender(std::string name) : Renderable(name)
{
}

ColliderRender::~ColliderRender()
{
}

void ColliderRender::Start()
{
	Renderable::Start();
}

void ColliderRender::Update(double dt)
{	
	Renderable::Update(dt);
}

void ColliderRender::End()
{
	Renderable::End();
}

void ColliderRender::Render()
{	
	if (!m_active || !mesh || mesh->vao < 1) return;

	//get handle for MgrGraphics
	MgrGraphics* mgrG = MgrGraphics::Instance();
	
	if (shader != mgrG->GetCurrShader())
		mgrG->UseShader(shader);

	//set uniforms for transform
	mgrG->SetUniform("model", t->GetModel());

	//set uniforms for material
	mgrG->SetUniform("material.albedo", material->albedo);
	for (int i = 0; i <= Material::COLOR7; ++i)
	{
		std::string cast = "material.colorMapEnabled[" + std::to_string(i) + "]";
		mgrG->SetUniform(cast.c_str(), i == 0); // only the first one
	}
	glBindTexture(GL_TEXTURE_2D, material->maps[Material::COLOR0]);

	DrawMesh();

	glBindTexture(GL_TEXTURE_2D, 0);
}

ColliderRender * ColliderRender::AttachPoints(std::vector<Vector3> pts)
{
	AttachMesh(MeshBuilder::GenerateLine("spline", pts, false));
	return this;
}