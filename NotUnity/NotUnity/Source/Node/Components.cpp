#include "Components.h"

#include "Components/Transform.h"
#include "Components/Renderable.h"
#include "Components/Sprite.h"
#include "Components/Text.h"
#include "Components/Camera.h"
#include "Components/Axis.h"

CompContainer::CompContainer(std::string name) : Node(name)
{
}

CompContainer::~CompContainer()
{
}

void CompContainer::Start()
{		
	Node::Start();
}

void CompContainer::Update(double dt)
{	
	Node::Update(dt);
}

void CompContainer::End()
{
	Node::End();
}