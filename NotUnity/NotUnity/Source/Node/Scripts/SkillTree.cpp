#include "SkillTree.h"

SkillTree::SkillTree(std::string name) : Node(name)
{
}

SkillTree::~SkillTree()
{
}

void SkillTree::Start()
{			
	Node::Start();
}

void SkillTree::Update(double dt)
{
	Node::Update(dt);
}

void SkillTree::End()
{
	Node::End();
}