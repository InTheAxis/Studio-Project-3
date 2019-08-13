#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	m_bounceScore = -1;
	m_elapsedTime = 0;

	//Physics code here
	m_speed = 1.f;
	m_gravity.Set(0, -9.8f, 0);
	
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	Math::InitRNG();
	MGR_GRAPHICS.Init();

	MGR_GRAPHICS.DefineNumLights(0);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -20, 20);
	MGR_GRAPHICS.DefineCustomProjection(ortho);
	//override default cam
	MGR_GRAPHICS.AttachCamera(&m_camera);
	glClearColor(0.9f, 0.9f, 0.85f, 1);

	m_gameState = 0;
	m_middleWorld[0] = 0.29f;
	m_middleWorld[1] = 1 - m_middleWorld[0];

	m_estimatedTime = 0;
	m_score1 = 0;
	m_score2 = 0;
	m_launchTime[0] = 0;
	m_launchTime[1] = 0;


	//init pools
	for (unsigned i = 0; i < NUM_ID; ++i)
	{
		m_goList[(GAMEOBJECT_ID)i].resize(POOL_SIZE[i]);
		for (unsigned j = 0; j < POOL_SIZE[i]; ++j)
		{
			m_goList[(GAMEOBJECT_ID)i][j] = new GameObject;
		}
	}

	//init meshes
	m_meshList.resize(NUM_MESHES);
	m_meshList[MESH_SPHERE] = *(MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 8, 8, 1));
	m_meshList[MESH_CUBE] = *(MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1));
	m_meshList[MESH_CYLINDER] = *(MeshBuilder::GenerateSphere("cylinder", Color(1, 1, 1), 8, 8, 1));
	m_meshList[MESH_QUAD] = *(MeshBuilder::GenerateQuad("quad", Color(1, 1, 1)));	
	m_meshList[MESH_QUAD].textureArray[1] = HelperResources::LoadTGA("Image//menu.tga");
	m_meshList[MESH_QUAD].textureArray[2] = HelperResources::LoadTGA("Image/winBlue.tga");
	m_meshList[MESH_QUAD].textureArray[3] = HelperResources::LoadTGA("Image//winOrange.tga");

	//init misc gameobjects
	m_ghost = new GameObject;
	*(m_ghost->AddComponent<CompMesh>()) = m_meshList[MESH_SPHERE];

	//init world
	BuildWorld();
}

GameObject* SceneCollision::GOFetch(SceneCollision::GAMEOBJECT_ID id)
{
	//fetch inactive object from pool
	for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
	{
		if (!(m_goList[id][i]->IsActive()))
			return m_goList[id][i];
	}
	//pool too small
	unsigned last = POOL_SIZE[id];
	POOL_SIZE[id] += 10;
	for (int i = 0; i < 10; ++i)
		m_goList[id].emplace_back(new GameObject);
	return m_goList[id][last];
}

GameObject * SceneCollision::GOSpawn(SceneCollision::GAMEOBJECT_ID id)
{
	GameObject* go = GOFetch(id);
	go->ActiveSelf(true);
	go->SetTag(static_cast<unsigned long long>(id));
	CompTransform* t = go->Transform();
	go->parent = nullptr;
	
	GameObject* child;
	CompKinematicBody* kb;
	switch (id)
	{
	case ID_BALL:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_SPHERE];
		go->GetComponent<CompMesh>()->color.Set(Math::RandFloatMinMax(0.3f, 0.9f), 0, Math::RandFloatMinMax(0.3f, 0.9f));
		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 1;
		kb->MAX_LIN_VEL = 50;
		kb->linearVel = 0;
		kb->gravScale = ballGrav;
		t->scale.Set(2, 2, 1);
		break;
	case ID_PORTAL:
		go->AddComponent<CompTracker>()->target = go;
	case ID_WALL:	
	case ID_HOLE:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_CUBE];
		go->GetComponent<CompMesh>()->color.Set(0.65f, 0.6f, 0.6f);
		t->scale.Set(1, 5, 1);
		break;
	case ID_FLIPPER:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_CUBE];
		go->GetComponent<CompMesh>()->color.Set(0.25f, 0.85f, 0.55f);
		t->scale.Set(4, 20, 10);
		kb = go->AddComponent<CompKinematicBody>();		
		kb->MAX_LIN_VEL = 0;
		kb->angVel = 0;			

		child = GOSpawn(ID_BUMPER);
		child->parent = go;		
		child->GetComponent<CompMesh>()->color.Set(0.25f, 0.65f, 0.55f);
		child->Transform()->translate.Set(0, 10, 0);
		child->Transform()->scale.Set(2, 2, 10);
		child = GOSpawn(ID_BUMPER);
		child->parent = go;
		child->GetComponent<CompMesh>()->color.Set(0.25f, 0.65f, 0.55f);
		child->Transform()->translate.Set(0, -10, 0);
		child->Transform()->scale.Set(2, 2, 10);
		break;
	case ID_PILLAR:
	case ID_BUMPER:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_CYLINDER];		
		go->GetComponent<CompMesh>()->color.Set(1, 0.7f, 0.1f);
		t->scale.Set(1, 1, 1);
		break;
	}

	return go;
}

void SceneCollision::Update(double dt)
{
	SceneBase::PreUpdate(dt);
	
	m_elapsedTime += dt;

	if (m_gameState == 0 && Application::IsKeyPressed(VK_SPACE))	
		m_gameState = 1;	
	else if (m_gameState > 1)
	{		
		if (m_elapsedTime > m_bounceScore + 1)
		{
			m_bounceScore = 0;
			Application::ReloadScene();
		}
		return;
	}

	DebugControl(dt);
	
	FlipperControl(dt);

	//Physics Simulation Section
	for (unsigned i = 0; i < POOL_SIZE[ID_BALL]; ++i)
	{
		GameObject* go = m_goList[ID_BALL][i];
		if (!(go->IsActive()))
			continue;

		HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt * m_speed, m_gravity * m_speed);
		HelperPhysics::KeepInBounds(go->Transform(), go->GetComponent<CompKinematicBody>(), 0, 0, m_worldWidth, m_worldHeight, 0);
		HelperPhysics::DespawnOffScreen(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 1);
	}

	CheckCollision(dt);	

	CheckScore(dt);

	SceneBase::PostUpdate(dt);
}

void SceneCollision::GORender(GameObject* go, MgrGraphics::SHADER shader, bool outline)
{
	CompTransform* t = go->Transform();
	if (!(go->IsActive()))
		return;

	if (go->parent)
	{
		CompTransform* tp = go->parent->Transform();
		m_modelStack.PushMatrix();
		m_modelStack.Translate(tp->translate.x, tp->translate.y, tp->translate.z);
		m_modelStack.Rotate(Math::RadianToDegree(atan2f(tp->dir.y, tp->dir.x)), 0, 0, 1);		
	}

	m_modelStack.PushMatrix();
	m_modelStack.Translate(t->translate.x, t->translate.y, t->translate.z);
	m_modelStack.Rotate(Math::RadianToDegree(atan2f(t->dir.y, t->dir.x)), 0, 0, 1);
	m_modelStack.Scale(t->scale.x, t->scale.y, t->scale.z);
	RenderMeshGO(go, shader, outline);
	m_modelStack.PopMatrix();

	if (go->parent)
	{
		CompTransform* tp = go->parent->Transform();
		m_modelStack.Scale(tp->scale.x, tp->scale.y, tp->scale.z);
		m_modelStack.PopMatrix();
	}
}

void SceneCollision::CheckCollision(double dt)
{
	for (unsigned i = 0; i < POOL_SIZE[ID_BALL]; ++i)
	{
		GameObject* go = m_goList[ID_BALL][i];
		if (!go->IsActive())
			continue;

		for (unsigned j = i + 1; j < POOL_SIZE[ID_BALL]; ++j)
		{
			GameObject* other = m_goList[ID_BALL][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallBall(go, other, 0, &penetration))
			{				
				HelperPhysics::ResolveCollisionBallBall(go, other, penetration);								
			}

			//float timeToCollide = HelperPhysics::GetTimeToCollideBallBall(go, other);			
			//if (timeToCollide > 0 && timeToCollide <= dt)
			//{				
			//	HelperPhysics::ResolveCollisionBallBall(go, other);
			//}
		}

		for (unsigned j = 0; j < POOL_SIZE[ID_PILLAR]; ++j)
		{
			GameObject* other = m_goList[ID_PILLAR][j];
			if (!other->IsActive())
				continue;

			Color* c = &(other->GetComponent<CompMesh>()->color);

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallPillar(go, other, 0, &penetration))
			{
				*c = HelperPhysics::Lerp(*c, Color(0.9f, 0.15f, 0.3f), 0.95f);
				HelperPhysics::ResolveCollisionBallPillar(go, other, penetration);
			}
			else
				*c = HelperPhysics::Lerp(*c, Color(1, 0.8f, 0.4f), 2 * dt);

			//float timeToCollide = HelperPhysics::GetTimeToCollideBallPillar(go, other);			
			//if (timeToCollide <= dt)
			//{	
			//	if (timeToCollide > 0)
			//		HelperPhysics::ResolveCollisionBallPillar(go, other);
			//}
		}

		for (unsigned j = 0; j < POOL_SIZE[ID_FLIPPER]; ++j)
		{
			GameObject* other = m_goList[ID_FLIPPER][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallWall(go, other, 0, &penetration, false))
			{
				HelperPhysics::ResolveCollisionBallFlipper(go, other, penetration, 0.8f);
			}

			//float timeToCollide = HelperPhysics::GetTimeToCollideBallWall(go, other);
			//m_estimatedTime = timeToCollide;
			//if (timeToCollide <= dt)
			//{
			//	if (timeToCollide > 0)
			//	{
			//		HelperPhysics::ResolveCollisionBallWall(go, other);					
			//	}
			//}
		}

		for (unsigned j = 0; j < POOL_SIZE[ID_BUMPER]; ++j)
		{
			GameObject* other = m_goList[ID_BUMPER][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallPillar(go, other, 0, &penetration, false))
			{				
				HelperPhysics::ResolveCollisionBallBumper(go, other, penetration, 0.8f);					
			}
		}

		for (unsigned j = 0; j < POOL_SIZE[ID_WALL]; ++j)
		{
			GameObject* other = m_goList[ID_WALL][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallWall(go, other, 0, &penetration))
			{
				HelperPhysics::ResolveCollisionBallWall(go, other, penetration, 0.9f);				
			}

			//float timeToCollide = HelperPhysics::GetTimeToCollideBallWall(go, other);
			//m_estimatedTime = timeToCollide;
			//if (timeToCollide <= dt)
			//{
			//	if (timeToCollide > 0)
			//	{
			//		HelperPhysics::ResolveCollisionBallWall(go, other);
			//		go->GetComponent<CompKinematicBody>()->gravScale = ballGrav;
			//	}
			//	//else if (timeToCollide > -1)
			//	//{
			//	//	HelperPhysics::SetToRest(go->GetComponent<CompKinematicBody>(), go->Transform(), dt, timeToCollide);
			//	//}
			//}
		}
		
		for (unsigned j = 0; j < POOL_SIZE[ID_PORTAL]; ++j)
		{
			GameObject* other = m_goList[ID_PORTAL][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallTrigger(go, other, 0, &penetration))
			{				
				HelperPhysics::ResolveCollisionBallGoal(go, other, penetration);				
			}
		}

		for (unsigned j = 0; j < POOL_SIZE[ID_HOLE]; ++j)
		{
			GameObject* other = m_goList[ID_HOLE][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallTrigger(go, other, 0, &penetration))
			{
				HelperPhysics::ResolveCollisionBallHole(go, other, penetration);
				if (other == m_hole[0])
					++m_score2;
				else if (other == m_hole[1])
					++m_score1;			
			}
		}

	}
}

void SceneCollision::CheckScore(double dt)
{
	if (m_score1 >= targetScore)
	{
		m_gameState = 2;
		m_bounceScore = m_elapsedTime;
	}
	else if (m_score2 >= targetScore)
	{
		m_gameState = 3;
		m_bounceScore = m_elapsedTime;
	}

	//change pillar scaling
	float scale[2] = { 3, 4 };	
	for (int i = 0; i < 2; ++i)
	{
		float add = (float)(i == 0 ? m_score1 : m_score2);
		add *= 0.25f;
		m_pillar[i * 3]->Transform()->scale.Set(scale[0] + add, scale[0] + add, 10);
		m_pillar[i * 3 + 1]->Transform()->scale.Set(scale[1] + add, scale[1] + add, 10);
		m_pillar[i * 3 + 2]->Transform()->scale.Set(scale[1] + add, scale[1] + add, 10);
	}
}

void SceneCollision::Launch(bool player)
{
	GameObject* bola;
	Vector3 baseVel(0, 100, 0);
	float size;
	float speed[2] = { 5, 10 };

	bola = GOSpawn(ID_BALL);
	bola->Transform()->translate.Set(m_worldWidth * m_middleWorld[player], m_worldHeight * 0.5f - 25, 0);
	Vector3 temp = baseVel + Vector3(Math::RandIntMinMax(0, 1) == 1 ? Math::RandFloatMinMax(-speed[1], -speed[0]) : Math::RandFloatMinMax(speed[0], speed[1]), 0, 0);
	bola->GetComponent<CompKinematicBody>()->linearVel = baseVel + Vector3(Math::RandIntMinMax(0, 1) == 1 ? Math::RandFloatMinMax(-speed[1], -speed[0]) : Math::RandFloatMinMax(speed[0], speed[1]), 0, 0);
	size = bola->Transform()->scale.x;
	bola->GetComponent<CompKinematicBody>()->mass = size * size * size;
}

void SceneCollision::Render()
{
	SceneBase::PreRender();	

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	if (m_gameState > 0)
	{
		GORender(m_ghost, MGR_GRAPHICS.SIMPLE);

		for (auto pool : m_goList)
		{
			for (auto go : pool.second)
			{
				GORender(go, MGR_GRAPHICS.SIMPLE);
			}
		}
	}
	//Exercise 8c: Render initial and final momentum

	//On screen text
	std::ostringstream ss;

	if (m_gameState == 2)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 20);
		m_modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		m_meshList[MESH_QUAD].textureArray[0] = m_meshList[MESH_QUAD].textureArray[2];
		MGR_GRAPHICS.RenderMesh(&m_meshList[MESH_QUAD], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
	}
	else if (m_gameState == 3)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 20);
		m_modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		m_meshList[MESH_QUAD].textureArray[0] = m_meshList[MESH_QUAD].textureArray[3];
		MGR_GRAPHICS.RenderMesh(&m_meshList[MESH_QUAD], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
	}		

	float textScale = 10;
	m_modelStack.PushMatrix();
	m_modelStack.Translate(m_worldWidth * 0.5f - textScale - 5, 15, 11);
	m_modelStack.Scale(textScale, textScale, 1);
	ss.precision(5); ss.str(""); ss.clear();
	ss << m_score1;
	RenderText(ss.str(), Color(0.05f, 0.4f, 0.7f));
	m_modelStack.PopMatrix();

	m_modelStack.PushMatrix();
	m_modelStack.Translate(m_worldWidth * 0.5f - textScale + 4, 15, 11);
	m_modelStack.Scale(textScale, textScale, 1);
	ss.precision(5); ss.str(""); ss.clear();
	ss << ":";
	RenderText(ss.str(), Color(0, 0, 0));
	m_modelStack.PopMatrix();

	m_modelStack.PushMatrix();
	m_modelStack.Translate(m_worldWidth * 0.5f - textScale + 14, 15, 11);
	m_modelStack.Scale(textScale, textScale, 1);
	ss.precision(5); ss.str(""); ss.clear(); 
	ss << m_score2;
	RenderText(ss.str(), Color(0.95f, 0.4f, 0.f));
	m_modelStack.PopMatrix();



	if (m_gameState == 0)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 20);
		m_modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		m_meshList[MESH_QUAD].textureArray[0] = m_meshList[MESH_QUAD].textureArray[1];
		MGR_GRAPHICS.RenderMesh(&m_meshList[MESH_QUAD], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
	}

	ss.precision(5); ss.str(""); ss.clear();
	ss << "Simul: " << m_speed;
	RenderTextOnScreen(ss.str(), 1.f, 9.5f);

	ss.precision(5); ss.str(""); ss.clear();
	ss << "GO Count:" << GameObject::count;
	RenderTextOnScreen(ss.str(), 1.f, 6.5f);

	ss.precision(5); ss.str(""); ss.clear();
	ss << "FPS:" << m_fps;
	RenderTextOnScreen(ss.str(), 1.f, 3.5f);
	
	RenderTextOnScreen("PPHY ASSN 2", 1.f, 0.5f);	

	SceneBase::PostRender();
}

void SceneCollision::Exit()
{
	//Cleanup GameObjects
	for (auto pool : m_goList)
	{
		for (auto go : pool.second)
		{
			go->ActiveSelf(false);
			delete go;
			go = nullptr;
		}
	}

	m_bounceScore = -1;
	m_elapsedTime = 0;
	m_gameState = 0;
	m_estimatedTime = 0;
	m_score1 = 0;
	m_score2 = 0;
	m_launchTime[0] = 0;
	m_launchTime[1] = 0;

	m_meshList.clear();

	delete m_ghost;
	m_ghost = nullptr;
	SceneBase::Exit();
}

void SceneCollision::DebugControl(double dt)
{
	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.5f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.5f;
	}

	double x, y;
	Application::GetCursorPos(&x, &y);
	x = x / Application::GetWindowWidth() * m_worldWidth;
	y = m_worldHeight - y / Application::GetWindowHeight() * m_worldHeight;

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		m_ghost->ActiveSelf(true);
		m_ghost->Transform()->translate.Set(static_cast<float>(x), static_cast<float>(y), 0);
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		GameObject* ball = GOSpawn(ID_BALL);
		ball->Transform()->translate = m_ghost->Transform()->translate;
		ball->GetComponent<CompKinematicBody>()->linearVel = m_ghost->Transform()->translate - Vector3(static_cast<float>(x), static_cast<float>(y), 0);	
		float size = ball->Transform()->scale.x;		
		ball->GetComponent<CompKinematicBody>()->mass = size * size * size;
		m_ghost->ActiveSelf(false);
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
	}
	//bunced input
	if (m_bounceScore < m_elapsedTime)
	{
		//m_bounceInput = m_elapsedTime + 1.f;
	}
}

void SceneCollision::FlipperControl(double dt)
{
	//hard coded cuz no time :(


	CompKinematicBody* kb1 = m_flipper[0]->GetComponent<CompKinematicBody>();
	CompKinematicBody* kb2 = m_flipper[1]->GetComponent<CompKinematicBody>();

	CompKinematicBody* kb3 = m_flipper[2]->GetComponent<CompKinematicBody>();
	CompKinematicBody* kb4 = m_flipper[3]->GetComponent<CompKinematicBody>();

	float maxAngVel = 5;
	double timeToHold = 1.0f;

	//make flipper slower if score defecit is big
	int defecit = 0;
	if (m_score2 > m_score1)
	{
		defecit = (m_score2 - m_score1) * 0.25f;
		kb3->MAX_ANG_VEL = maxAngVel - defecit;
		kb4->MAX_ANG_VEL = maxAngVel - defecit;
	}
	else if (m_score2 > m_score1)
	{
		defecit = (m_score1 - m_score2) * 0.25f;
		kb1->MAX_ANG_VEL = maxAngVel - defecit;
		kb2->MAX_ANG_VEL = maxAngVel - defecit;
	}
	else
	{
		kb1->MAX_ANG_VEL = kb2->MAX_ANG_VEL =
		kb3->MAX_ANG_VEL = kb4->MAX_ANG_VEL = maxAngVel;
	}


	if (Application::IsKeyPressed('W'))
	{
		kb1->angVel = kb1->MAX_ANG_VEL;
		kb2->angVel = -kb2->MAX_ANG_VEL;
		m_launchTime[0] += dt;
		if (m_launchTime[0] > timeToHold)
		{
			m_launchTime[0] = 0;
			Launch(false);
		}
	}
	else
	{
		kb1->angVel = -kb1->MAX_ANG_VEL;
		kb2->angVel = kb2->MAX_ANG_VEL;		
		m_launchTime[0] = 0;	
	}

	if (Application::IsKeyPressed(VK_UP))
	{
		kb3->angVel = kb3->MAX_ANG_VEL;
		kb4->angVel = -kb4->MAX_ANG_VEL;
		m_launchTime[1] += dt;
		if (m_launchTime[1] > timeToHold)
		{
			m_launchTime[1] = 0;
			Launch(true);
		}
	}
	else
	{
		kb3->angVel = -kb3->MAX_ANG_VEL;
		kb4->angVel = kb4->MAX_ANG_VEL;
		m_launchTime[1] = 0;
	}

	HelperPhysics::UpdateKinematics(kb1, m_flipper[0]->Transform(), dt * m_speed, m_gravity * m_speed);
	HelperPhysics::UpdateKinematics(kb2, m_flipper[1]->Transform(), dt * m_speed, m_gravity * m_speed);
	HelperPhysics::UpdateKinematics(kb3, m_flipper[2]->Transform(), dt * m_speed, m_gravity * m_speed);
	HelperPhysics::UpdateKinematics(kb4, m_flipper[3]->Transform(), dt * m_speed, m_gravity * m_speed);

	if (kb1->rad > Math::PI * 0.8f)
	{		
		kb1->rad = Math::PI * 0.8f;
		m_flipper[0]->Transform()->dir.Set(cosf(kb1->rad), sinf(kb1->rad), 0);
		kb2->rad = Math::PI * 0.2f;
		m_flipper[1]->Transform()->dir.Set(cosf(kb2->rad), sinf(kb2->rad), 0);

		kb1->angVel = 0;
		kb2->angVel = 0;
	}
	else if (kb1->rad < Math::HALF_PI * 0.4f)
	{		
		kb1->rad = Math::HALF_PI * 0.4f;
		m_flipper[0]->Transform()->dir.Set(cosf(kb1->rad), sinf(kb1->rad), 0);
		kb2->rad = Math::HALF_PI * 1.6f;
		m_flipper[1]->Transform()->dir.Set(cosf(kb2->rad), sinf(kb2->rad), 0);

		kb1->angVel = 0;
		kb2->angVel = 0;
	}

	if (kb3->rad > Math::PI * 0.8f)
	{
		kb3->rad = Math::PI * 0.8f;
		m_flipper[2]->Transform()->dir.Set(cosf(kb3->rad), sinf(kb3->rad), 0);
		kb4->rad = Math::PI * 0.2f;
		m_flipper[3]->Transform()->dir.Set(cosf(kb4->rad), sinf(kb4->rad), 0);

		kb3->angVel = 0;
		kb4->angVel = 0;
	}
	else if (kb3->rad < Math::HALF_PI * 0.4f)
	{
		kb3->rad = Math::HALF_PI * 0.4f;
		m_flipper[2]->Transform()->dir.Set(cosf(kb3->rad), sinf(kb3->rad), 0);
		kb4->rad = Math::HALF_PI * 1.6f;
		m_flipper[3]->Transform()->dir.Set(cosf(kb4->rad), sinf(kb4->rad), 0);

		kb3->angVel = 0;
		kb4->angVel = 0;
	}
}

void SceneCollision::BuildWorld()
{		
	const float scaleZ = 10;
	//walls
	{
		GameObject* wall;
		GameObject* pillar;		
		Vector3 middleOfArea;
		float angle;
		Vector3 pos;
		Vector3 normal;
		Vector3 scale;

		for (int h = 0; h < 2; ++h)
		{
			/////////////////////////////////////////////walls

			middleOfArea.Set(m_worldWidth * m_middleWorld[h], m_worldHeight * 0.5f + 5, 0);

			for (int i = 0; i < 360; i += 10)
			{
				angle = Math::DegreeToRadian((float)i);
				normal.Set(cosf(angle), sinf(angle));
				pos = middleOfArea + normal * 35;
				scale.Set(3, 6.5f, scaleZ);

				if ((i >= 70 && i <= 110) || (i >= 250 && i <= 290))
					continue;				

				wall = GOSpawn(ID_WALL);
				wall->Transform()->dir = normal;
				wall->Transform()->scale = scale;
				wall->Transform()->translate = pos;
			}


			//////////////////////////////////////////triggers

			scale.Set(30, 45, scaleZ + 1);

			m_hole[h] = GOSpawn(ID_HOLE);
			m_hole[h]->Transform()->translate = middleOfArea - Vector3(0, 43, 0);
			m_hole[h]->Transform()->scale = scale;
			m_hole[h]->GetComponent<CompMesh>()->color.Set(0.3f, 0.25f, 0.2f);

			m_portal[h] = GOSpawn(ID_PORTAL);
			m_portal[h]->Transform()->translate = middleOfArea + Vector3(0, 43, 0);
			m_portal[h]->Transform()->scale = scale;

			if (h)
			{
				m_portal[0]->GetComponent<CompMesh>()->color.Set(0.06f, 0.55f, 0.7f);
				m_portal[1]->GetComponent<CompMesh>()->color.Set(1, 0.55f, 0.25f);
				m_portal[1]->GetComponent<CompTracker>()->target = m_portal[0];			
				m_portal[0]->GetComponent<CompTracker>()->target = m_portal[1];			
			}

			//////////////////////////////////////////flippers

			m_flipper[h * 2] = GOSpawn(ID_FLIPPER);
			m_flipper[h * 2]->Transform()->translate = middleOfArea + Vector3(-14, -15, 0);
			//m_flipper[h * 2]->GetComponent<CompKinematicBody>()->cog.Set(m_flipper[h * 2]->Transform()->scale.y * 0.5f, 0, 0);
			m_flipper[h * 2 + 1] = GOSpawn(ID_FLIPPER);
			m_flipper[h * 2 + 1]->Transform()->translate = middleOfArea + Vector3(14, -15, 0);
			//m_flipper[h * 2 + 1]->GetComponent<CompKinematicBody>()->cog.Set(-m_flipper[h * 2 + 1]->Transform()->scale.y * 0.5f, 0, 0);

			//////////////////////////////////////////pillars
						
			m_pillar[h * 3] = GOSpawn(ID_PILLAR);
			m_pillar[h * 3]->Transform()->translate = middleOfArea + Vector3(0, 5, 0);
			m_pillar[h * 3]->Transform()->scale.Set(3, 3, scaleZ);
			
			m_pillar[h * 3 + 1] = GOSpawn(ID_PILLAR);
			m_pillar[h * 3 + 1]->Transform()->translate = middleOfArea + Vector3(-18, 9, 0);
			m_pillar[h * 3 + 1]->Transform()->scale.Set(4, 4, scaleZ);

			m_pillar[h * 3 + 2] = GOSpawn(ID_PILLAR);
			m_pillar[h * 3 + 2]->Transform()->translate = middleOfArea + Vector3(18, 9, 0);
			m_pillar[h * 3 + 2]->Transform()->scale.Set(4, 4, scaleZ);
		}

	}
}