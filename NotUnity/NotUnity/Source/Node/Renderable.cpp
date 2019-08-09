#include "Renderable.h"
#include "../Manager/MgrGraphics.h"

void Renderable::Start()
{
	Node::Start();
}

void Renderable::Update(double dt)
{
	Render();
	Node::Update(dt);
}

void Renderable::End()
{
	Node::End();
}

void Renderable::Render()
{
	if (!mesh || mesh->vao < 1) return;

	MgrGraphics* mgrG = MgrGraphics::Instance();
	
	mgrG->SetUniform("material.albedo", mesh->material.albedo);
	for (int i = 0; i <= Material::COLOR7; ++i)
	{
		std::string cast = "material.colorMapEnabled[" + std::to_string(i) + "]";
		mgrG->SetUniform(cast.c_str(), mesh->material.maps[i] > 0);
	}
	glBindTexture(GL_TEXTURE_2D, mesh->material.maps[Material::COLOR0]);

	DrawMesh();
}

Renderable* Renderable::AttachMesh(Mesh* mesh)
{
	this->mesh = mesh;
	return this;
}

Renderable* Renderable::AttachMaterial(Material material)
{
	this->mesh->material = material;
	return this;
}

void Renderable::DrawMesh()
{
	DrawMesh(mesh->indexSize, 0);
}

void Renderable::DrawMesh(unsigned count, unsigned offset)
{
	glBindVertexArray(mesh->vao);
	glDrawElements(mesh->drawMode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
	glBindVertexArray(0);
}
