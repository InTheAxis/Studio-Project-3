#include "Scene.h"
#include "Renderable.h"
#include "../Utility/Graphics/MeshBuilder.h"

void Scene::Start()
{	
	Material* placeholder = new Material("placeholder");
	Transform* t = new Transform;

	AddChild<Renderable>("tri")->AttachMesh(MeshBuilder::GenerateQuad("tri"))->AttachMaterial(MgrGraphics::Instance()->GetCachedMaterial("placeholder"))->AttachTransform(t);
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
