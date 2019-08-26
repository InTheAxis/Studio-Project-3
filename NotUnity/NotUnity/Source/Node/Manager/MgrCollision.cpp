#include "MgrCollision.h"

#include "../Components/Collider.h"

MgrCollision::MgrCollision(std::string name) : Manager<MgrCollision>(name)
{
}

MgrCollision::~MgrCollision()
{
	allColls.clear();
}

void MgrCollision::Start()
{	
	Node::Start();
}

void MgrCollision::Update(double dt)
{
	unsigned size = allColls.size();
	for (unsigned i = 0; i < size; ++i)
	{
		if (!(allColls[i]->IsActive()))
			continue;
		for (unsigned j = i + 1; j < size; ++j)
		{
			if (!(allColls[j]->IsActive()))
				continue;
			allColls[i]->CheckCollision(allColls[j]);			
		}
	}

	Node::Update(dt);
}

void MgrCollision::End()
{
	Node::End();
}

void MgrCollision::RegisterColl(Collider * c)
{
	for (auto col : allColls)
		if (col == c)
			return;

	allColls.emplace_back(c);
}

void MgrCollision::UnregisterColl(Collider * c)
{
	for (auto it = allColls.begin(); it != allColls.end(); ++it)
	{
		if (*it == c)
		{
			allColls.erase(it);
			return;
		}
	}
}
