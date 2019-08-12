#include "Sprite.h"
#include "../Manager/MgrGraphics.h"
#include "../../Utility/Math/MyMath.h"

Sprite::Sprite(std::string name) : Renderable(name)
{
	currFrame = 0;
	currTime = 0.0;
	startFrame = 0;
	endFrame = 5;
	repeat = true;
	animTime = 1.f;
	ended = false;	
	selectedAnim = 0;
}

Sprite::~Sprite()
{
}

void Sprite::Start()
{
	frameTime = animTime / Math::Max(1, endFrame - startFrame + 1);
	Renderable::Start();
}

void Sprite::Update(double dt)
{	
	if (!ended)
	{
		currTime += dt;		
		currFrame = Math::Min(endFrame, startFrame + (int)(currTime / frameTime));

		if (currTime >= animTime)
		{
			if (repeat)
			{
				currTime = 0.f;
				currFrame = startFrame;
			}			
			else
				ended = true;
		}
	}

	Renderable::Update(dt);
}

void Sprite::End()
{
	Renderable::End();
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
		mgrG->SetUniform(cast.c_str(), material->maps[i] > 0 && i == selectedAnim);
	}
	glBindTexture(GL_TEXTURE_2D, material->maps[Material::COLOR0 + selectedAnim]);

	DrawMesh(6, 6 * currFrame);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::SwitchAnimation(int idx)
{
	selectedAnim = idx;
}
