#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "Vector3.h"
#include "MatrixStack.h"
#include "CompSpriteAnim.h"
#include "CameraBase.h"

class ParticleObject
{
public:
	ParticleObject();
	~ParticleObject();	

	Vector3 pos, vel, scale;
	float rotation, rotationSpeed;

	double lifeTime;

	bool active;	
};

class ParticleList
{
public:
	ParticleList();
	~ParticleList();
	

	void Init(CompSpriteAnim* mesh);
	void Update(double dt);
	void Render(MS* modelStackRef, CameraBase* cameraRef);
private:
	std::vector<ParticleObject*> m_arr;

	int m_particleCount;
	int MAX_PARTICLE;
	Vector3 m_gravity;
	int numPerFrame;
	Vector3 m_emissionPointMin, m_emissionPointMax;
	Vector3 m_velMin, m_velMax;
	float m_rotateSpeedMax, m_rotateSpeedMin;
	double m_lifeSpan;
	float m_particleScale;

	CompSpriteAnim* mesh;

	ParticleObject* GetParticle();
};

#endif
