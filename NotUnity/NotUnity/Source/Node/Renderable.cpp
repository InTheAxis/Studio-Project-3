#include "Renderable.h"

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
	DrawMesh();
}

void Renderable::AttachMesh(Mesh* mesh)
{
	this->mesh = mesh;
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
