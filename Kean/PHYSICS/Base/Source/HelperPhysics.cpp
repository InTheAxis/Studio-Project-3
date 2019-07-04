#include "HelperPhysics.h"

float HelperPhysics::IsOverlapping2(GameObject * go, GameObject * other)
{
	CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
	CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();

	Vector3 relativeVel = kb1->linearVel - kb2->linearVel;
	Vector3 relativeDir = go->Transform()->translate - other->Transform()->translate;
	float combinedRadius = go->Transform()->scale.x + other->Transform()->scale.x;

	if (kb1->linearVel.Dot(relativeDir) > 0)
		return -1;

	float a = relativeVel.Dot(relativeVel);
	float b = 2 * relativeVel.Dot(relativeDir);
	float c = relativeDir.Dot(relativeDir) - combinedRadius * combinedRadius;

	float discriminant = (b * b) - 4 * a * c;

	if (discriminant < 0)
		return -1;

	float root1 = (-b + sqrt(discriminant)) / (2 * a);
	float root2 = (-b - sqrt(discriminant)) / (2 * a);

	if (root1 < 0) return root2;
	if (root2 < 0) return root1;
	else return root1;
}
