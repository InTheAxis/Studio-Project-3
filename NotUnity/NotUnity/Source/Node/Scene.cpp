#include "Scene.h"
#include "GameObj.h"

void Scene::Start()
{	
	AddChild<GameObj>("temp");
	Node::Start();
}

void Scene::Update(double dt)
{
	Node::Update(dt);
}

void Scene::End()
{
	Node::End();
}
