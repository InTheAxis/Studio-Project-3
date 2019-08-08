#include "Scene.h"
#include "Renderable.h"
#include "../Utility/Graphics/MeshBuilder.h"

void Scene::Start()
{	
	AddChild<Renderable>("tri")->AttachMesh(MeshBuilder::GenerateQuad("tri"));
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
