#include "Components.h"

#include "Components/Transform.h"
#include "Components/Renderable.h"
#include "Components/Sprite.h"
#include "Components/Text.h"
#include "Components/Camera.h"
#include "Components/Axis.h"

Components::Components(std::string name) : Node(name)
{
}

Components::~Components()
{
}

void Components::Start()
{		
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