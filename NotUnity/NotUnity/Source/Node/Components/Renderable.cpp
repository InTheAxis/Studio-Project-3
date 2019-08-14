#include "Renderable.h"
#include "../Manager/MgrGraphics.h"
#include "../Manager/MgrGameObj.h"
#include "../GameObj.h"

Renderable::Renderable(std::string name) 
	: Node(name) 
	, mesh(nullptr)
	, material(nullptr)
	, t(nullptr)
	, shader(MgrGraphics::DEFAULT)
{
	MgrGameObj::Instance()->RegisterRenderable(this);
}

Renderable::~Renderable()
{
}

void Renderable::Start()
{
	t = gameObject->GetComp<Transform>();
	Node::Start();
}

void Renderable::Update(double dt)
{	
	Node::Update(dt);
}

void Renderable::End()
{
	Node::End();
}

void Renderable::Render()
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

	DrawMesh();

	glBindTexture(GL_TEXTURE_2D, 0);
}

Renderable* Renderable::AttachMesh(Mesh* mesh)
{
	this->mesh = mesh;
	return this;
}

Renderable* Renderable::AttachMaterial(Material* material)
{
	this->material = material;
	return this;
}

Renderable * Renderable::SelectShader(MgrGraphics::SHADER shader)
{
	this->shader = shader;
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
