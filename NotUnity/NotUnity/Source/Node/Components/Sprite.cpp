#include "Sprite.h"
#include "../../Manager/MgrGraphics.h"

void Sprite::Start()
{
	Renderable::Start();
}

void Sprite::Update(double dt)
{	
	Renderable::Update(dt);
	Node::Update(dt);
}

void Sprite::End()
{
	Renderable::End();
	Node::End();
}

void Sprite::Render()
{
	if (!mesh || mesh->vao < 1) return;

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
		mgrG->SetUniform(cast.c_str(), material->maps[i] > 0);
	}
	glBindTexture(GL_TEXTURE_2D, material->maps[Material::COLOR0]);

	//TODO sprite animatiion

	DrawMesh();

	glBindTexture(GL_TEXTURE_2D, 0);
}