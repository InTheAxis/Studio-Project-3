#include "Scene.h"
#include "GameObj.h"

void Scene::Start()
{	
	AddChild<GameObj>("temp")->Create(Components::JSON);
	//Node::Start(); //gameobjects start themselves
}

void Scene::Update(double dt)
{
	Node::Update(dt);
}

void Scene::End()
{
	Node::End();
}
