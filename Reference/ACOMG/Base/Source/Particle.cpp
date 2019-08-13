#include "Particle.h"

#include "MgrGraphics.h"

ParticleObject::ParticleObject()
	: active(false)
	, pos(0, 0, 0)
	, scale(1, 1, 1)
	, vel(0, 0, 0)
	, rotation(0)
	, rotationSpeed(1)	
{
}

ParticleObject::~ParticleObject()
{
}

ParticleList::ParticleList()
	: m_particleCount(0)
	, m_gravity(0, -9.8f, 0)
	, MAX_PARTICLE(1000)
	, numPerFrame(1)
	, m_emissionPointMin(0, 0, 0)
	, m_emissionPointMax(0, 0, 0)
	, m_velMax(0, 0, 0)
	, m_velMin(0, 0, 0)
	, m_rotateSpeedMax(0)
	, m_rotateSpeedMin(0)
	, m_lifeSpan(1)
	, m_particleScale(1)
{
}

ParticleList::~ParticleList()
{
	while (m_arr.size() > 0)
	{
		ParticleObject* p = m_arr.back();
		delete p;
		m_arr.pop_back();
	}
}

void ParticleList::Init(CompSpriteAnim* mesh, int effect)
{	
	for (long i = 0; i < 10; ++i)
	{
		ParticleObject* newParticle = new ParticleObject;
		newParticle->active = false;
		m_arr.emplace_back(newParticle);
	}
	this->mesh = mesh;

	switch (effect)
	{
	case 1:
		//stage smoke effect
		numPerFrame = 2;
		m_emissionPointMax = Vector3(100, 210, -1000);
		m_emissionPointMin = Vector3(-100, 210, -1100);
		m_velMax = Vector3(15, 0, 10);
		m_velMin = Vector3(-15, 0, -10);
		m_rotateSpeedMax = 100;
		m_rotateSpeedMin = -100;
		m_gravity = Vector3(0, 0, 100);
		m_lifeSpan = 10.f;
		m_particleScale = 40;
		break;
	case 2:
		//fire
		numPerFrame = 12;
		m_emissionPointMax = Vector3(40, 220, -341);
		m_emissionPointMin = Vector3(40, 220, -341);
		m_velMax = Vector3(10, 150, 10);
		m_velMin = Vector3(-20, 10, -10);
		m_rotateSpeedMax = 10;
		m_rotateSpeedMin = -10;
		m_gravity = Vector3(40, 200, 0);
		m_lifeSpan = 0.5f;
		m_particleScale = 1;
		break;
	case 3:
		//fire
		numPerFrame = 12;
		m_emissionPointMax = Vector3(-40, 220, -341);
		m_emissionPointMin = Vector3(-40, 220, -341);
		m_velMax = Vector3(20, 150, 10);
		m_velMin = Vector3(-10, 10, -10);
		m_rotateSpeedMax = 10;
		m_rotateSpeedMin = -10;
		m_gravity = Vector3(-40, 200, 0);
		m_lifeSpan = 0.5f;
		m_particleScale = 1;
		break;
	}
}

void ParticleList::Update(double dt)
{
	for (int i = 0; i < numPerFrame; ++i)
	{
		if (m_particleCount < MAX_PARTICLE)
		{
			ParticleObject* p = GetParticle();
			p->scale.Set(m_particleScale, m_particleScale, m_particleScale);
			p->vel.x = Math::RandFloatMinMax(m_velMin.x, m_velMax.x);
			p->vel.y = Math::RandFloatMinMax(m_velMin.y, m_velMax.y);
			p->vel.z = Math::RandFloatMinMax(m_velMin.z, m_velMax.z);
			p->rotationSpeed = Math::RandFloatMinMax(m_rotateSpeedMin, m_rotateSpeedMax);
			p->pos.x = Math::RandFloatMinMax(m_emissionPointMin.x, m_emissionPointMax.x);
			p->pos.y = Math::RandFloatMinMax(m_emissionPointMin.y, m_emissionPointMax.y);
			p->pos.z = Math::RandFloatMinMax(m_emissionPointMin.z, m_emissionPointMax.z);
			p->lifeTime = 0;
		}
	}

	for (auto p : m_arr)
	{
		if (!p->active)
			continue;
		p->vel += m_gravity * (float)dt;
		p->pos += p->vel * (float)dt;
		p->rotation += p->rotationSpeed * (float)dt;
		p->lifeTime += dt;
		if (p->lifeTime > m_lifeSpan)
		{
			p->active = false;
			m_particleCount--;
		}
	}

	mesh->Update(dt);
	mesh->m_anim->animActive = true;
}

void ParticleList::Render(MS* modelStackRef, CameraBase* cameraRef, bool depth)
{	
	for (auto it = m_arr.rbegin(); it != m_arr.rend(); ++it)
	{
		ParticleObject* p = *it;
		if (!p->active)
			continue;
		modelStackRef->PushMatrix();
		modelStackRef->Translate(p->pos.x, p->pos.y, p->pos.z);
		//use billboard shader means no z rotations
		//modelStackRef->Rotate(Math::RadianToDegree(atan2(cameraRef->position.x - p->pos.x, cameraRef->position.z - p->pos.z)), 0, 1, 0);
		//modelStackRef->Rotate(p->rotation, 0, 0, 1);
		modelStackRef->Scale(p->scale.x, p->scale.y, p->scale.z);
		MgrGraphics::Instance().RenderSpriteAnim(mesh, depth ? MGR_GRAPHICS.DEPTH : MGR_GRAPHICS.BILLBOARD_UNLIT);
		modelStackRef->PopMatrix();
	}	
}

ParticleObject * ParticleList::GetParticle()
{
	for (auto p : m_arr)
	{
		if (!p->active)
		{
			p->active = true;
			m_particleCount++;
			return p;
		}
	}

	//add more to pool
	for (long i = 0; i < 10; ++i)
	{
		ParticleObject* newParticle = new ParticleObject;
		m_arr.emplace_back(newParticle);
	}
	ParticleObject* ret = m_arr.back();
	ret->active = true;
	m_particleCount++;
	return ret;
}
