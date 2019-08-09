#include "Transform.h"

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
	MS model;
	model.Translate(translate.x, translate.y, translate.z);
	model.Scale(scale.x, scale.y, scale.z);
	return model.Top();
}
