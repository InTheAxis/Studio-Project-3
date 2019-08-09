#include "Components.h"

#include "Components/Transform.h"
#include "Components/Renderable.h"

void Components::Start()
{
	AddChild<Transform>();
	Node::Start();
}

void Components::Update(double dt)
{
	Node::Update(dt);
}

void Components::End()
{
	Node::End();
}
