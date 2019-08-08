#include "Scene.h"
#include "../Utility/Graphics/MeshBuilder.h"

void Scene::Start()
{
	Node::Start();

	MeshBuilder::GenerateTriangle("tri");
}

void Scene::Update(double dt)
{
	Node::Update(dt);
}

void Scene::End()
{
	Node::End();
}
