#include "Text.h"
#include "../../Manager/MgrGraphics.h"

void Text::Start()
{
	Node::Start();
}

void Text::Update(double dt)
{	
	Node::Update(dt);
}

void Text::End()
{
	Node::End();
}

void Text::Render()
{
	if (!mesh || mesh->vao < 1 || content == "") return;

	glDisable(GL_DEPTH_TEST);

	//get handle for MgrGraphics
	MgrGraphics* mgrG = MgrGraphics::Instance();
	
	if (shader != mgrG->GetCurrShader())
		mgrG->UseShader(shader);

	//set uniforms for material
	mgrG->SetUniform("material.albedo", material->albedo);
	for (int i = 0; i <= Material::COLOR7; ++i)
	{
		std::string cast = "material.colorMapEnabled[" + std::to_string(i) + "]";
		mgrG->SetUniform(cast.c_str(), i == 0); // only the first one
	}
	glBindTexture(GL_TEXTURE_2D, material->maps[Material::COLOR0]);

	Mtx44 charModel;
	for (unsigned i = 0; i < content.length(); ++i)
	{
		charModel.SetToTranslation(i * textSpacing, 0, 0);
		mgrG->SetUniform("model", t->GetModel() * charModel);
		DrawMesh(6, (unsigned)content[i] * 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

Text* Text::SetText(std::string text)
{
	content = text;
	return this;
}

Text * Text::SetOnScreen(bool onScreen)
{
	this->onScreen = onScreen;
	return this;
}
