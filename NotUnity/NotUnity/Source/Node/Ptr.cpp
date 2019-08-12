#include "Ptr.h"

Ptr::Ptr(std::string name) 
	: Node(name)
	, ref(nullptr) 
{
}

Ptr::~Ptr()
{
}

void Ptr::Start()
{
	Node::Start();
}

void Ptr::Update(double dt)
{
	Node::Update(dt);
}

void Ptr::End()
{
	Node::End();
}