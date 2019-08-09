#include "Transform.h"
#include "../../Manager/MgrGraphics.h"

void Transform::Start()
{
	Node::Start();
}

void Transform::Update(double dt)
{
	Node::Update(dt);
}

void Transform::End()
{
	Node::End();
}

Mtx44 Transform::GetModel()
{	
	MS* model = MgrGraphics::Instance()->GetModelStack();
	model->PushMatrix();	
	model->Translate(translate.x, translate.y, translate.z);
	model->Rotate(rotate.z, 0, 0, 1);
	model->Scale(scale.x, scale.y, scale.z);
	Mtx44 temp = model->Top();
	model->PopMatrix();	
	return temp;
}
