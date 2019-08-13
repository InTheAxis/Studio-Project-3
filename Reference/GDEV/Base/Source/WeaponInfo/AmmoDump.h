#pragma once

#include "Vector3.h"
#include "Mesh.h"

class CAmmoDump
{
public:
	CAmmoDump();
	virtual ~CAmmoDump();

	// Get modelMesh
	Mesh* GetModelMesh(void) const;
	// Set modelMesh
	void SetModelMesh(Mesh* modelMesh);

	// Get position
	Vector3 GetPosition(void) const;
	// Set position
	void SetPosition(Vector3 vPosition);

	// Render the Ammo Dump
	void Render(void);

protected:
	Vector3 vPosition;

	Mesh* modelMesh;
};