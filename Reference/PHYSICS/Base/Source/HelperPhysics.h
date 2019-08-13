#ifndef HELPER_PHYSICS_H
#define HELPER_PHYSICS_H

#include "GameObject.h"
#include "CompKinematicBody.h"
#include "CompTransform.h"
#include "CompTracker.h"

namespace HelperPhysics
{
	inline void UpdateKinematics(CompKinematicBody* kb, CompTransform* t, double dt, Vector3 gravity)
	{
		//linear, using momentum
		kb->prevLinearVel = kb->linearVel;				
		kb->linearVel += gravity * kb->gravScale * static_cast<float>(dt);
		//kb->linearVel += (1.f / kb->mass) * kb->impulse * static_cast<float>(dt);
		//kb->impulse.SetZero();
		//angular
		kb->prevAngVel = kb->angVel;
		//kb->angVel += kb->torque / kb->momentOfInertia * static_cast<float>(dt);
		//kb->torque = 0;
		////limits
		if (kb->linearVel.LengthSquared() > kb->MAX_LIN_VEL * kb->MAX_LIN_VEL)
			kb->linearVel = kb->linearVel.Normalized() * kb->MAX_LIN_VEL;
		if (kb->angVel > kb->MAX_ANG_VEL)
			kb->angVel = kb->MAX_ANG_VEL;
		if (kb->angVel < -kb->MAX_ANG_VEL)
			kb->angVel = -kb->MAX_ANG_VEL;
		////update dir
		kb->rad = atanf(t->dir.y / t->dir.x);
		if (t->dir.x < 0)
			kb->rad += Math::PI;
		kb->rad += 0.5f * (kb->prevAngVel + kb->angVel) * static_cast<float>(dt);
		t->dir.Set(cosf(kb->rad), sinf(kb->rad), 0);
		//update pos
		t->translate += 0.5f * (kb->prevLinearVel + kb->linearVel) * static_cast<float>(dt);	
		t->translate.z = 1;
	}

	inline void SetToRest(CompKinematicBody* kb, CompTransform* t, double dt, double timeToCollide)
	{
		t->translate -= kb->linearVel * (dt - timeToCollide);	
		if (kb->linearVel.IsZero())
			kb->linearVel.SetZero();
		kb->gravScale = 0;
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
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
		CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();
		Vector3 vec = (Vector3(t2->translate.x, t2->translate.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));

		//dir check
		if (vec.Dot(kb1->linearVel - kb2->linearVel) < 0)
			return false;

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

	inline bool IsOverlappingBallWall(GameObject* go, GameObject* other, float offset = 0, Vector3* penetrationVec = nullptr, bool dirCheck = true)
	{
		if (go == other) return false;
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		CompKinematicBody* kb = go->GetComponent<CompKinematicBody>();
		Vector3 vec = (Vector3(t2->translate.x, t2->translate.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));
		float disp, minDist;
		t2->dir.Normalize();
		Vector3 parallel(-t2->dir.y, t2->dir.x);
		Vector3 wallMin, wallMax;
		Vector3 penDir;

		//check wall normal		
		disp = -vec.Dot(t2->dir);
		penDir = t2->dir;
		if (disp < 0)
			penDir = -penDir;

		minDist = t1->scale.x + t2->scale.x * 0.5f + offset;
		if ((!dirCheck || kb->linearVel.Dot(penDir) <= 0) && Math::FAbs(disp) < minDist)
		{
			//check if ball is within wall length
			wallMin = t2->translate + parallel * t2->scale.y * 0.5f;
			wallMax = t2->translate + parallel * -t2->scale.y * 0.5f;
			if ((wallMin - t1->translate + penDir * (t1->scale.x + t2->scale.x * 0.5f)).Dot(
				wallMax - t1->translate + penDir * (t1->scale.x + t2->scale.x * 0.5f)) <= 0)
			{
				*penetrationVec = (Math::FAbs(disp) - minDist) * penDir;
				return true;
			}
		}		

		////check wall parallel
		//disp = -vec.Dot(parallel);
		//penDir = parallel;
		//if (disp < 0)
		//	penDir = -penDir;
		////dir check
		//minDist = t1->scale.x + t2->scale.y * 0.5f + offset;
		//if (kb->linearVel.Dot(penDir) <= 0 && Math::FAbs(disp) < minDist)
		//{
		//	//only check if ball is within wall width
		//	wallMin = t2->translate + t2->dir * t2->scale.x * 0.5f;
		//	wallMax = t2->translate + t2->dir * -t2->scale.x * 0.5f;
		//	if ((wallMin - t1->translate + penDir * (t1->scale.x + t2->scale.y * 0.5f)).Dot(
		//		wallMax - t1->translate + penDir * (t1->scale.x + t2->scale.y * 0.5f)) <= 0)
		//	{
		//		*penetrationVec = 2 * (Math::FAbs(disp) - minDist) * penDir;
		//		return true;
		//	}
		//}

		return false;
	}

	inline bool IsOverlappingBallPillar(GameObject* go, GameObject* other, float offset = 0, Vector3* penetrationVec = nullptr, bool dirCheck = true)
	{
		if (go == other) return false;
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		CompKinematicBody* kb = go->GetComponent<CompKinematicBody>();
		Vector3 vec;
		if (!(other->parent))
			vec = (Vector3(t2->translate.x, t2->translate.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));
		else
		{
			CompTransform* p = other->parent->Transform();
			float radian = other->parent->GetComponent<CompKinematicBody>()->rad;
			Vector3 relPos(cosf(radian) * t2->translate.x - sinf(radian) * t2->translate.y, sinf(radian) * t2->translate.x + cosf(radian) * t2->translate.y, 0);
			vec = (Vector3(p->translate.x + relPos.x, p->translate.y + relPos.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));
		}

		//dir check
		if (dirCheck && vec.Dot(kb->linearVel) < 0)
			return false;		

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

	inline bool IsOverlappingBallTrigger(GameObject* go, GameObject* other, float offset = 0, Vector3* penetrationVec = nullptr)
	{
		if (go == other) return false;
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();	
		CompKinematicBody* kb = go->GetComponent<CompKinematicBody>();
		Vector3 vec = (Vector3(t2->translate.x, t2->translate.y, 0) - Vector3(t1->translate.x, t1->translate.y, 0));
		float disp, minDist;
		t2->dir.Normalize();
		Vector3 parallel(-t2->dir.y, t2->dir.x);
		Vector3 wallMin, wallMax;
		Vector3 penDir;

		//check wall normal		
		disp = -vec.Dot(t2->dir);
		penDir = t2->dir;
		if (disp < 0)
			penDir = -penDir;

		minDist = -t1->scale.x + t2->scale.x * 0.5f + offset;
		if (kb->linearVel.Dot(penDir) <= 0 && Math::FAbs(disp) < minDist)
		{
			wallMin = t2->translate + parallel * t2->scale.y * 0.5f;
			wallMax = t2->translate + parallel * -t2->scale.y * 0.5f;
			if ((wallMin - t1->translate + penDir * (t1->scale.x + t2->scale.x * 0.5f)).Dot(
				wallMax - t1->translate + penDir * (t1->scale.x + t2->scale.x * 0.5f)) <= 0)
			{
				*penetrationVec = 2 * (Math::FAbs(disp) - minDist) * penDir;
				return true;
			}
		}

		//check wall parallel
		disp = -vec.Dot(parallel);
		penDir = parallel;
		if (disp < 0)
			penDir = -penDir;
		//dir check
		minDist = -t1->scale.x + t2->scale.y * 0.5f + offset;
		if (kb->linearVel.Dot(penDir) <= 0 && Math::FAbs(disp) < minDist)
		{
			//only check if ball is within wall width
			wallMin = t2->translate + t2->dir * t2->scale.x * 0.5f;
			wallMax = t2->translate + t2->dir * -t2->scale.x * 0.5f;
			if ((wallMin - t1->translate + penDir * (t1->scale.x + t2->scale.y * 0.5f)).Dot(
				wallMax - t1->translate + penDir * (t1->scale.x + t2->scale.y * 0.5f)) <= 0)
			{
				*penetrationVec = 2 * (Math::FAbs(disp) - minDist) * penDir;
				return true;
			}
		}

		return false;
	}


	inline float GetTimeToCollideBallBall(GameObject* go, GameObject* other)
	{
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
		CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();

		Vector3 relativeVel = kb1->linearVel - kb2->linearVel;
		relativeVel.z = 0;
		Vector3 relativeDir = go->Transform()->translate - other->Transform()->translate;
		relativeDir.z = 0;
		float combinedRadius = go->Transform()->scale.x + other->Transform()->scale.x;

		//dir check
		if (relativeVel.Dot(relativeDir) > 0) return -1;

		//get abc of derived quadratic
		float a = relativeVel.Dot(relativeVel);
		float b = 2 * relativeVel.Dot(relativeDir);
		float c = relativeDir.Dot(relativeDir) - combinedRadius * combinedRadius;

		//calc discriminant
		float discriminant = (b * b) - 4 * a * c;

		//trivial rejection, no real roots
		if (discriminant < 0) return -1;

		//determine roots
		float root1 = (-b + sqrt(discriminant)) / (2 * a);
		float root2 = (-b - sqrt(discriminant)) / (2 * a);

		//determine return val
		float ret;
		if (root1 <= 0) ret = root2;
		else ret = Math::Min(root1, root2);

		return ret;
	}

	inline float GetTimeToCollideBallWall(GameObject* go, GameObject* other)
	{		
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();

		Vector3 relativeVel = kb1->linearVel;
		relativeVel.z = 0;
		Vector3 relativeDir = t2->translate - t1->translate;
		relativeDir.z = 0;		
		float combinedRadius = t1->scale.x + 0.5f * t2->scale.x;
		Vector3 normal = t2->dir;
		Vector3 parallel(-normal.y, normal.x);
		Vector3 wallMin, wallMax;

		//flip normal
		if (relativeDir.Dot(t2->dir) < 0) normal = -normal;

		//speed check and dir check
		if (relativeVel.IsZero() || relativeVel.Dot(normal) < 0) return -1;

		//check if within line segment
		wallMin = t2->translate + parallel * t2->scale.y * 0.5f;
		wallMax = t2->translate + parallel * -t2->scale.y * 0.5f;
		if ((wallMin - t1->translate + normal * (t1->scale.x + t2->scale.x * 0.5f)).Dot(
			wallMax - t1->translate + normal * (t1->scale.x + t2->scale.x * 0.5f)) <= 0)
		{
			//calculate time using dist/speed
			float retN;
			retN = (relativeDir.Dot(normal) - combinedRadius) / relativeVel.Dot(normal);

			return retN;
		}
	}

	inline float GetTimeToCollideBallPillar(GameObject* go, GameObject* other)
	{
		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();		

		Vector3 relativeVel = kb1->linearVel;
		relativeVel.z = 0;
		Vector3 relativeDir;

		if (!(other->parent))
			relativeDir = t1->translate - t2->translate;
		else
		{
			CompTransform* p = other->parent->Transform();
			float radian = other->parent->GetComponent<CompKinematicBody>()->rad;
			Vector3 relPos(cosf(radian) * t2->translate.x - sinf(radian) * t2->translate.y, sinf(radian) * t2->translate.x + cosf(radian) * t2->translate.y, 0);
			relativeDir = p->translate + relPos;
			relativeDir = t1->translate - relativeDir;
		}

		relativeDir.z = 0;
		float combinedRadius = go->Transform()->scale.x + other->Transform()->scale.x;

		//dir check
		if (relativeVel.Dot(relativeDir) > 0) return -1;

		//get abc of derived quadratic
		float a = relativeVel.Dot(relativeVel);
		float b = 2 * relativeVel.Dot(relativeDir);
		float c = relativeDir.Dot(relativeDir) - combinedRadius * combinedRadius;

		//calc discriminant
		float discriminant = (b * b) - 4 * a * c;

		//trivial rejection, no real roots
		if (discriminant < 0) return -1;

		//determine roots
		float root1 = (-b + sqrt(discriminant)) / (2 * a);
		float root2 = (-b - sqrt(discriminant)) / (2 * a);

		//determine return val
		float ret;
		if (root1 <= 0) ret = root2;
		else ret = Math::Min(root1, root2);

		return ret;
	}


	inline void ResolveCollisionBallBall(GameObject* go, GameObject* other, Vector3 penetration = Vector3())
	{
		go->Transform()->translate -= 0.2f * penetration;

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

		Vector3 relativeVel = kb2->prevLinearVel - kb1->prevLinearVel;
		Vector3 relativeDir = t2->translate - t1->translate;
		float massRatio = 2 / (kb1->mass + kb2->mass);
		float distRatio = relativeVel.Dot(relativeDir) / relativeDir.LengthSquared();
		//project onto tangent and normal of collision contact surface, apply 1d collision to normals and add back to tangents
		//prevent divide by 0
		if (relativeDir.IsZero())
			t1->translate -= 0.05f * penetration;

		kb1->linearVel = kb1->prevLinearVel - kb2->mass * massRatio * distRatio * -relativeDir;
		kb2->linearVel = kb2->prevLinearVel - kb1->mass * massRatio * distRatio * relativeDir;
	}

	inline void ResolveCollisionBallFlipper(GameObject* go, GameObject* other, Vector3 penetration = Vector3(), float speedMultiplier = 1)
	{
		go->Transform()->translate -= 0.8f * penetration;

		CompTransform* t1 = go->Transform();
		CompTransform* t2 = other->Transform();
		CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
		CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();
		Vector3 normal = other->Transform()->dir;

		if (kb1->linearVel.Dot(normal) > 0)
			normal = -normal;

		Vector3 relativeDir = (t2->translate - t1->translate);
		relativeDir.z = 0;

		Vector3 relativeVel;
		Vector3 perpen = -relativeDir.Dot(normal) * normal;
		float displacementHorizontal = (-relativeDir - perpen).Length();
		
		//multiply intial normal vel before reflecting
		if (kb2->angVel == 0)
			kb1->linearVel = kb1->linearVel.Dot(normal) * normal * speedMultiplier + kb1->linearVel.Dot(Vector3(-normal.y, normal.x)) * Vector3(-normal.y, normal.x);

		relativeVel = kb1->linearVel - normal * (displacementHorizontal * Math::FAbs(kb2->angVel));


		kb1->prevLinearVel = kb1->linearVel;
		kb1->linearVel = relativeVel - (2 * relativeVel.Dot(normal)) * normal;
	}

	inline void ResolveCollisionBallWall(GameObject* go, GameObject* other, Vector3 penetration = Vector3(), float speedMultiplier = 1)
	{
		go->Transform()->translate -= 0.8f * penetration;

		CompKinematicBody* kb = go->GetComponent<CompKinematicBody>();
		Vector3 normal = other->Transform()->dir;

		if (kb->linearVel.Dot(normal) > 0)
			normal = -normal;

		Vector3 relativeDir = (other->Transform()->translate - go->Transform()->translate);		

		//multiply intial normal vel before reflecting
		kb->linearVel = kb->linearVel.Dot(normal) * normal * speedMultiplier + kb->linearVel.Dot(Vector3(-normal.y, normal.x)) * Vector3(-normal.y, normal.x);

		kb->prevLinearVel = kb->linearVel;
		kb->linearVel = kb->prevLinearVel - (2 * kb->prevLinearVel.Dot(normal)) * normal;						
	}

	inline void ResolveCollisionBallPillar(GameObject* go, GameObject* other, Vector3 penetration = Vector3(), float speedMultiplier = 1)
	{		
		go->Transform()->translate -= 0.2f * penetration;

		CompTransform *t1, *t2;
		t1 = go->Transform();
		t2 = other->Transform();
		CompKinematicBody *kb;
		kb = go->GetComponent<CompKinematicBody>();		
		Vector3 normal;
		if (!penetration.IsZero())
			normal = (-penetration).Normalized();

		//multiply intial normal vel before reflecting
		kb->linearVel = kb->linearVel.Dot(normal) * normal * speedMultiplier + kb->linearVel.Dot(Vector3(-normal.y, normal.x)) * Vector3(-normal.y, normal.x);

		kb->prevLinearVel = kb->linearVel;	
		kb->linearVel = kb->prevLinearVel - (2 * kb->prevLinearVel.Dot(normal)) * normal;
		kb->linearVel *= speedMultiplier;
	}

	inline void ResolveCollisionBallBumper(GameObject* go, GameObject* other, Vector3 penetration = Vector3(), float speedMultiplier = 1)
	{
		go->Transform()->translate -= 0.8f * penetration;

		CompTransform *t1, *t2;
		t1 = go->Transform();
		t2 = other->Transform();
		CompKinematicBody *kb;
		kb = go->GetComponent<CompKinematicBody>();

		Vector3 normal;
		if (!penetration.IsZero())
			normal = (-penetration).Normalized();		

		//hack way of getting flipper ang vel
		if (other->parent && other->parent->GetComponent<CompKinematicBody>())
		{
			float angVel = Math::FAbs(other->parent->GetComponent<CompKinematicBody>()->angVel);
			if (angVel == 0)
			{
				//multiply intial normal vel before reflecting
				kb->linearVel = kb->linearVel.Dot(normal) * normal * speedMultiplier + kb->linearVel.Dot(Vector3(-normal.y, normal.x)) * Vector3(-normal.y, normal.x);				
			}

			Vector3 relativeVel;
			relativeVel = kb->linearVel - normal * angVel * (other->parent->Transform()->scale.y * 0.5f + go->Transform()->scale.x);
			kb->prevLinearVel = kb->linearVel;
			kb->linearVel = relativeVel - (2 * relativeVel.Dot(normal)) * normal;
		}
		else
		{
			kb->prevLinearVel = kb->linearVel;
			kb->linearVel = kb->prevLinearVel - (2 * kb->prevLinearVel.Dot(normal)) * normal;
		}
	}

	inline void ResolveCollisionBallHole(GameObject* go, GameObject* other, Vector3 penetration)
	{
		go->ActiveSelf(false);
	}

	inline void ResolveCollisionBallGoal(GameObject* go, GameObject* other, Vector3 penetration)
	{
		CompTransform *t1, *t2;
		t1 = go->Transform();
		t2 = other->GetComponent<CompTracker>()->target->Transform();
		CompKinematicBody* kb = go->GetComponent<CompKinematicBody>();
		
		Vector3 offset = t1->translate - other->Transform()->translate;		

		t1->translate = t2->translate + offset;

		kb->prevLinearVel = kb->linearVel;
		kb->linearVel = -kb->prevLinearVel;
	}

	template <typename T>
	inline T Lerp(T start, T end, float weight)
	{
		Math::Clamp(weight, 0.f, 1.f);
		return (1.f - weight) * start + weight * end;
	}		
};

#endif