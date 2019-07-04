#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"

#include <vector>
using namespace std;

class Mesh;

class CEnemy3D :
	public GenericEntity
{
protected:
	Mesh* modelMesh;

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 position, target, up;
	Vector3 view, right;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;
	
	double m_dSpeed;
	double m_dAcceleration;
	float m_fElapsedTimeBeforeUpdate;

public:
	CEnemy3D(Mesh* _modelMesh);
	virtual ~CEnemy3D();

	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	//getter setters
	void SetPos(const Vector3& pos);	
	void SetTarget(const Vector3& target);
	void SetUp(const Vector3& up);	
	void SetBoundary(Vector3 max, Vector3 min);	
	void SetTerrain(GroundEntity* m_pTerrain);
	Vector3 GetPos(void) const;	
	Vector3 GetTarget(void) const;	
	Vector3 GetUp(void) const;	
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);

	void UpdateTargetRightUp(Vector3& target, Vector3& right, Vector3& up);
};

namespace Create
{
	CEnemy3D* Enemy3D(const std::string& _meshName,
						const Vector3& _position,
						const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
