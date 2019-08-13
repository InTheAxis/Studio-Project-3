#include "AmmoDump.h"

#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"

CAmmoDump::CAmmoDump()
	: modelMesh(NULL)
{
	vPosition.Set(0.0f, 0.0f, 0.0f);
}


CAmmoDump::~CAmmoDump()
{
}

// Get modelMesh
Mesh* CAmmoDump::GetModelMesh(void) const
{
	return modelMesh;
}

// Set modelMesh
void CAmmoDump::SetModelMesh(Mesh* modelMesh)
{
	this->modelMesh = modelMesh;
}

// Get position
Vector3 CAmmoDump::GetPosition(void) const
{
	return vPosition;
}

// Set position
void CAmmoDump::SetPosition(Vector3 vPosition)
{
	this->vPosition = vPosition;
}

// Render this AmmoDump
void CAmmoDump::Render(void)
{
	Mesh* modelMesh;
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
		modelMesh = MeshBuilder::GetInstance()->GetMesh("AMMO_DUMP");
		modelStack.Translate(vPosition.x, vPosition.y, vPosition.z);
		modelStack.Scale(1.0f, 1.0f, 1.0f);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}