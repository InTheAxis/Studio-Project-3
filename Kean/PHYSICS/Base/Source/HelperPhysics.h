#ifndef HELPER_PHYSICS_H
#define HELPER_PHYSICS_H

#include "GameObject.h"
#include "CompKinematicBody.h"
#include "CompTransform.h"

namespace HelperPhysics
{
	inline void UpdateKinematics(CompKinematicBody* kb, CompTransform* t, double dt)
	{
		//linear, using momentum
		kb->prevLinearVel = kb->linearVel;		
		//kb->linearVel += (1.f / kb->mass) * kb->force * static_cast<float>(dt);
		//kb->force.SetZero();
		//angular
		//kb->prevAngVel = kb->angVel;
		//kb->angVel += kb->torque / kb->momentOfInertia * static_cast<float>(dt);
		//kb->torque = 0;
		////limits
		if (kb->linearVel.LengthSquared() > kb->MAX_LIN_VEL * kb->MAX_LIN_VEL)
			kb->linearVel = kb->linearVel.Normalized() * kb->MAX_LIN_VEL;
		//if (kb->angVel > kb->MAX_ANG_VEL)
		//	kb->angVel = kb->MAX_ANG_VEL;
		//if (kb->angVel < -kb->MAX_ANG_VEL)
		//	kb->angVel = -kb->MAX_ANG_VEL;
		////update dir
		//float radian = atanf(t->dir.y / t->dir.x);
		//if (t->dir.x < 0)
		//	radian += Math::PI;
		//radian += 0.5f * (kb->prevAngVel + kb->angVel) * static_cast<float>(dt);
		//t->dir.Set(cosf(radian), sinf(radian), 0);
		//update pos
		t->translate += 0.5f * (kb->prevLinearVel + kb->linearVel) * static_cast<float>(dt);	
		t->translate.z = 1;
	}

	inline void WrapAround(CompTransform* t, float minX, float minY, float maxX, float maxY, float offset)
	{
		t->translate.x = Math::Wrap(t->translate.x, -t->scale.x + minX, maxX + offset + t->scale.x);
		t->translate.y = Math::Wrap(t->translate.y, -t->scale.y + minY, maxY + offset + t->scale.y);
	}
	
	inline void KeepInBounds(CompTransform* t, CompKinematicBody* kb, float minX, float minY, float maxX, float maxY, float offset)
	{
		if (t->translate.x < t->scale.x + minX - offset ||
			t->translate.x > -t->scale.x + maxX + offset)
		{
			t->translate.x = Math::Clamp(t->translate.x , t->scale.x + minX - offset, -t->scale.x + maxX + offset);
			kb->linearVel.x *= -1;
		}

		if (t->translate.y < t->scale.y + minY - offset ||
			t->translate.y > -t->scale.y + maxY + offset)
		{
			t->translate.y = Math::Clamp(t->translate.y, t->scale.y + minY- offset, -t->scale.y + maxY + offset);
			kb->linearVel.y *= -1;
		}
	}

	inline void DespawnOffScreen(CompTransform* t, float minX, float minY, float maxX, float maxY, float offset)
	{
		if (t->translate.x < -t->scale.x + minX - offset ||
			t->translate.x > t->scale.x + maxX + offset ||
			t->translate.y < -t->scale.y + minY - offset ||
			t->translate.y > t->scale.y + maxY + offset)
			t->parent->ActiveSelf(false);
	}

	inline bool IsOverlappingBallBall(GameObject* go, GameObject* other, float offset = 0, Vector3* penetrationVec = nullptr)
	{
		if (go == other) return false;
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		Vector3 vec = (Vector3(t2->translate.x, t2->translate.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));
		float length = vec.Length();
		float radii = t1->scale.x + t2->scale.x + offset;
		if (length < radii)
		{	
			if (Math::FIsEqual(length, 0))
				*penetrationVec = radii * Vector3(1, 0, 0);
			else
				*penetrationVec = (radii - length) * vec.Normalized();

			return true;
		}
		return false;
	}

	inline float GetTimeToCollideBallBall(GameObject* go, GameObject* other)
	{
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
		CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();

		Vector3 relativeVel = kb1->linearVel - kb2->linearVel;
		Vector3 relativeDir = go->Transform()->translate - other->Transform()->translate;
		float combinedRadius = go->Transform()->scale.x + other->Transform()->scale.x;

		if (relativeVel.Dot(relativeDir) > 0)
			return -1;

		float a = relativeVel.Dot(relativeVel);
		float b = 2 * relativeVel.Dot(relativeDir);
		float c = relativeDir.Dot(relativeDir) - combinedRadius * combinedRadius;
			
		float discriminant = (b * b) - 4 * a * c;

		if (discriminant < 0)
			return -1;

		float root1 = (-b + sqrt(discriminant)) / (2 * a);
		float root2 = (-b - sqrt(discriminant)) / (2 * a);

		if (root1 <= 0) return root2;			
		return Math::Min(root1, root2);
	}

	inline bool IsOverlappingBallWall(GameObject* go, GameObject* other, float offset = 0, Vector3* penetrationVec = nullptr)
	{
		if (go == other) return false;



		return false;
	}

	inline float GetTimeToCollideBallWall(GameObject* go, GameObject* other)
	{
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
		CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();

		return 0;
	}

	inline void ResolveCollisionBallBall(GameObject* go, GameObject* other, Vector3 penetration)
	{
		go->Transform()->translate -= 0.5f * penetration;
		other->Transform()->translate += 0.5f * penetration;

		CompTransform *t1, *t2;
		t1 = go->Transform();
		t2 = other->Transform();
		CompKinematicBody *kb1, *kb2;
		kb1 = go->GetComponent<CompKinematicBody>();
		kb2 = other->GetComponent<CompKinematicBody>();
		kb1->prevLinearVel = kb1->linearVel;
		kb2->prevLinearVel = kb2->linearVel;
		//1d collision, simul eqn with conservation of KE and conservation of momentum

		//kb1->linearVel = (kb1->mass - kb2->mass) / (kb1->mass + kb2->mass) * kb1->prevLinearVel + (2 * kb2->mass) / (kb1->mass + kb2->mass) * kb2->prevLinearVel;
		//kb2->linearVel = (2 * kb1->mass) / (kb1->mass + kb2->mass) * kb1->prevLinearVel + (kb2->mass - kb1->mass) / (kb1->mass + kb2->mass) * kb2->prevLinearVel;

		//project onto tangent and normal of collision contact surface, apply 1d collision to normals and add back to tangents

		kb1->linearVel = kb1->prevLinearVel -
			((2 * kb2->mass) / (kb1->mass + kb2->mass)) *
			(((kb1->prevLinearVel - kb2->prevLinearVel).Dot(t1->translate - t2->translate)) / (t1->translate - t2->translate).LengthSquared()) *
			(t1->translate - t2->translate);
		kb2->linearVel = kb2->prevLinearVel -
			((2 * kb1->mass) / (kb1->mass + kb2->mass)) *
			(((kb2->prevLinearVel - kb1->prevLinearVel).Dot(t2->translate - t1->translate)) / (t2->translate - t1->translate).LengthSquared()) *
			(t2->translate - t1->translate);
	}

	inline void ResolveCollisionBallWall(GameObject* go, GameObject* other, Vector3 penetration)
	{
		go->Transform()->translate -= 0.5f * penetration;
		other->Transform()->translate += 0.5f * penetration;
	}

	inline Vector3 Lerp(float start, float end, float weight)
	{
		Math::Clamp(weight, 0.f, 1.f);
		return (1.f - weight) * start + weight * end;
	}	
	inline Vector3 Lerp(Vector3 start, Vector3 end, float weight)
	{
		Math::Clamp(weight, 0.f, 1.f);
		return (1.f - weight) * start + weight * end;
	}
};

#endif