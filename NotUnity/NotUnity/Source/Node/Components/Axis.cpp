#include "Axis.h"

Axis::Axis(std::string name) : Node(name)
{
}

Axis::~Axis()
{
}

void Axis::Start()
{
	Reset();
	Node::Start();
}

void Axis::Update(double dt)
{
	Node::Update(dt);
}

void Axis::End()
{
	Node::End();
}

void Axis::Reset()
{
	view.Set(0, 0, -1);
	up.Set(0, 1, 0);
	right = view.Cross(up).Normalized();
}
