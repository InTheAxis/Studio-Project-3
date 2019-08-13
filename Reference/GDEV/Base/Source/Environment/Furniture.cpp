#include "Furniture.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"

CFurniture::CFurniture(Mesh* _modelMesh)
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_fElapsedTimeBeforeUpdate(0.0f)
	, hooked(false)
{
	this->modelMesh = _modelMesh;
	Init();
}


CFurniture::~CFurniture()
{
}

void CFurniture::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 0.0;

	// Add to EntityManager
//	EntityManager::GetInstance()->AddEntity(this);

}

// Reset this player instance to default
void CFurniture::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CFurniture::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CFurniture::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CFurniture::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CFurniture::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CFurniture::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CFurniture::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CFurniture::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CFurniture::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CFurniture::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CFurniture::Update(double dt)
{
	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;
	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	Constrain();

	// This is RealTime Loop control
	// Update the target once every 5 seconds. 
	// Doing more of these calculations will not affect the outcome.
	m_fElapsedTimeBeforeUpdate += dt;
	if (m_fElapsedTimeBeforeUpdate > 5.0f)
	{
		cout << m_fElapsedTimeBeforeUpdate << endl;
		m_fElapsedTimeBeforeUpdate = 0.0f;
		if (position.z > 400.0f)
			target.z = position.z * -1;
		else if (position.z < -400.0f)
			target.z = position.z * -1;
	}
}

// Constrain the position within the borders
void CFurniture::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CFurniture::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

CFurniture* Create::Furniture(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CFurniture* result = new CFurniture(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(true);
	result->SetAABB(Vector3(10.0f,5.0f, 10.0f), Vector3(-10.0f, -5.0f, -10.0f));
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}