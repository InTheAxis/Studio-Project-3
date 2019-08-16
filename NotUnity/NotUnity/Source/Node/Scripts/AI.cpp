#include "AI.h"
#include "../Manager/MgrGameObj.h"
#include "../../Node/GameObj.h"

AI::AI(std::string name) : Node(name), health(0), damage(0)
{
}

AI::~AI()
{
}

void AI::Start()
{	
	//gameObject->AddComp<Text>(); 
	//OR

	Node::Start();
}

void AI::Update(double dt)
{
	Node::Update(dt);
}

void AI::End()
{
	Node::End();
}

void AI::setHealth(int health)
{
	this->health = health;
}
