#include "ColorSpot.h"
#include "../Manager/MgrGameObj.h"
#include "../Manager/MgrGraphics.h"
#include "../../Node/GameObj.h"
#include "../../Node/Components/Transform.h"

ColorSpot::ColorSpot(std::string name) 
	: Node(name)
	, radius(2)
{
}

ColorSpot::~ColorSpot()
{
}

void ColorSpot::Start()
{		
	t = gameObject->GetTransform();	
	Node::Start();
}

void ColorSpot::Update(double dt)
{
	Node::Update(dt);
}

void ColorSpot::End()
{
	Node::End();
}

void ColorSpot::SetUniform(int idx)
{
	std::string temp;
	temp = "pt[" + std::to_string(idx) + "].pos";
	MgrGraphics::Instance()->SetUniform(temp, t->translate, MgrGraphics::COLOR_SPOT);
	temp = "pt[" + std::to_string(idx) + "].radius";
	MgrGraphics::Instance()->SetUniform(temp, radius, MgrGraphics::COLOR_SPOT);
}