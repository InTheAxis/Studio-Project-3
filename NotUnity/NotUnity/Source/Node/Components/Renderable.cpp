#include "Renderable.h"
#include "../Manager/MgrGraphics.h"
#include "../Manager/MgrGameObj.h"
#include "../GameObj.h"

Renderable::Renderable(std::string name) 
	: Node(name) 
	, mesh(nullptr)
	, material(nullptr)
	, t(nullptr)
	, shader(MgrGraphics::UNLIT)	
	, renderPass(RENDER_PASS::FINAL)
	, scrollSpeed(0,0)
	, cullBackFace(true)
{
}

Renderable::~Renderable()
{
	mesh = nullptr;
	material = nullptr;
	t = nullptr;
}

void Renderable::Start()
{	
	MgrGameObj::Instance()->RegisterRenderable(this);
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
	if (!m_active || !mesh || mesh->vao < 1) return;

	//get handle for MgrGraphics
	MgrGraphics* mgrG = MgrGraphics::Instance();
	
	if (shader != mgrG->GetCurrShader())
		mgrG->UseShader(shader);

	//set uniforms for transform
	mgrG->SetUniform("model", t->GetModel());
	if (scrollSpeed.IsZero())
		mgrG->SetUniform("scrollAmt", Vector3());
	else
		mgrG->SetUniform("scrollAmt", m_lifetime * Vector3(scrollSpeed.x, scrollSpeed.y, 1));


	//set uniforms for material
	mgrG->SetUniform("material.albedo", material->albedo);
	for (int i = 0; i <= Material::COLOR13; ++i)
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

Renderable* Renderable::SetToScroll(Vector2 scrollSpeed)
{
	this->scrollSpeed = scrollSpeed;
	return this;
}

Renderable* Renderable::ToggleCullFace(bool on)
{
	cullBackFace = on;
	return this;
}

RENDER_PASS Renderable::GetRenderPass() const
{
	return renderPass;
}

Renderable* Renderable::SetRenderPass(RENDER_PASS rp)
{
	renderPass = rp;
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
