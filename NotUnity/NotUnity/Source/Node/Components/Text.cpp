#include "Text.h"
#include "../Manager/MgrGraphics.h"

Text::Text(std::string name) 
	: Renderable(name)
	, content("")
	, color(1,1,1,1)
	, fontSize(1)
	, onScreen(true)
	, align(-1)
{
}

Text::~Text()
{
}

void Text::Start()
{
	Renderable::Start();	
}

void Text::Update(double dt)
{	
	Renderable::Update(dt);	
}

void Text::End()
{
	Renderable::End();
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
	mgrG->SetUniform("material.albedo", color); //let text override albedo
	for (int i = 0; i <= Material::COLOR7; ++i)
	{
		std::string cast = "material.colorMapEnabled[" + std::to_string(i) + "]";
		mgrG->SetUniform(cast.c_str(), i == 0); // only the first one
	}
	glBindTexture(GL_TEXTURE_2D, material->maps[Material::COLOR0]);

	//set proj and view
	if (onScreen)
	{
		mgrG->GetProjStack()->PushMatrix();
		mgrG->SetProjOrtho(50, -100, 100);
		Mtx44 no_view; no_view.SetToIdentity();
		mgrG->SetUniform("view", no_view);
		mgrG->SetUniform("proj", mgrG->GetProjStack()->Top());
	}

	//set model & draw
	MS charModel;	
	if (!align)
		charModel.Translate((content.length() - 1) * -0.5f * textSpacing * fontSize, 0, 0);
	else if (align == 1)
		charModel.Translate(content.length() * -textSpacing * fontSize, 0, 0);
	else
		charModel.Translate(textSpacing * fontSize, 0, 0);
	for (unsigned i = 0; i < content.length(); ++i)
	{
		charModel.PushMatrix();
		charModel.Translate(i * textSpacing * fontSize, 0, 0);
		charModel.Scale(fontSize, fontSize, 1);
		mgrG->SetUniform("model", t->GetModel() * charModel.Top());
		DrawMesh(6, (unsigned)content[i] * 6);
		charModel.PopMatrix();
	}

	//reset proj and view
	if (onScreen)
	{
		mgrG->GetProjStack()->PopMatrix();		
		mgrG->SetUniform("view", mgrG->GetView());
		mgrG->SetUniform("proj", mgrG->GetProjStack()->Top());
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}

Text* Text::SetText(std::string text)
{
	content = text;
	return this;
}

Text* Text::SetColor(Vector4 col)
{
	color = col;
	return this;
}

Text * Text::SetSize(float size)
{
	fontSize = size;
	return this;
}

Text* Text::SetOnScreen(bool onScreen)
{
	this->onScreen = onScreen;
	return this;
}

Text * Text::SetAlignment(int align)
{
	this->align = align;
	return this;
}