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

	m_bounceInput = -1;
	m_elapsedTime = 0;

	//Physics code here
	m_speed = 1.f;
	
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	Math::InitRNG();

	MGR_GRAPHICS.DefineNumLights(0);
	Mtx44 ortho;
	ortho.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -20, 20);
	MGR_GRAPHICS.DefineCustomProjection(ortho);
	//override default cam
	MGR_GRAPHICS.AttachCamera(&m_camera);
	glClearColor(0, 0, 0, 1);

	/*m1 = m2 = 0;
	u1 = u2 = v1 = v2 = 0;*/
	m_timerStarted = false;
	m_timeTaken1 = m_estimatedTime = 0;

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
	m_meshList[MESH_SPHERE] = *(MeshBuilder::GenerateSphere("sphere", Color(1,1,1), 10, 10, 1));
	m_meshList[MESH_CUBE] = *(MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1));

	//init misc gameobjects
	m_ghost = new GameObject;
	*(m_ghost->AddComponent<CompMesh>()) = m_meshList[MESH_SPHERE];

	//init world
	{
		Vector3 middleOfWalls, offset;
		const float scale = 25;
		GameObject* wall;
		middleOfWalls.Set(40, m_worldHeight * 0.5f, 1);
		//left
		wall = GOSpawn(ID_WALL);
		offset.Set(-scale, 0, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(1, 0, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//right
		wall = GOSpawn(ID_WALL);
		offset.Set(scale, 0, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(-1, 0, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//bot
		wall = GOSpawn(ID_WALL);
		offset.Set(0, -scale, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(0, 1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//bot
		wall = GOSpawn(ID_WALL);
		offset.Set(0, scale, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(0, -1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//lefttop
		wall = GOSpawn(ID_WALL);
		offset.Set(-scale * 0.75f, scale * 0.75f, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(1, -1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//righttop
		wall = GOSpawn(ID_WALL);
		offset.Set(scale * 0.75f, scale * 0.75f, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(-1, -1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//leftbot
		wall = GOSpawn(ID_WALL);
		offset.Set(-scale * 0.75f, -scale * 0.75f, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(1, 1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//rightbot
		wall = GOSpawn(ID_WALL);
		offset.Set(scale * 0.75f, -scale * 0.75f, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(-1, 1, 0);
		wall->Transform()->scale.Set(1, scale, 1);
		//middle
		wall = GOSpawn(ID_WALL);
		offset.Set(0, 0, 0);
		wall->Transform()->translate = middleOfWalls + offset;
		wall->Transform()->dir.Set(0, 1, 0);
		wall->Transform()->scale.Set(1, scale * 0.5f, 1);
	}
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
	

	CompKinematicBody* kb;
	switch (id)
	{
	case ID_BALL:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_SPHERE];
		go->GetComponent<CompMesh>()->color.Set(Math::RandFloatMinMax(0.1f, 1), 0, Math::RandFloatMinMax(0.1f, 1));
		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 1;
		kb->MAX_LIN_VEL = 50;
		kb->linearVel = 0;
		t->scale.Set(1, 1, 1);
		break;
	case ID_WALL:
		*(go->AddComponent<CompMesh>()) = m_meshList[MESH_CUBE];
		go->GetComponent<CompMesh>()->color.Set(0, 0.5f, 0);
		t->scale.Set(1, 5, 1);
		break;
	}

	return go;
}

void SceneCollision::Update(double dt)
{
	SceneBase::PreUpdate(dt);
	
	m_elapsedTime += dt;

	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}	

	double x, y;
	Application::GetCursorPos(&x, &y);
	x = x / Application::GetWindowWidth() * m_worldWidth;
	y = m_worldHeight - y / Application::GetWindowHeight() * m_worldHeight;

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		m_ghost->ActiveSelf(true);
		m_ghost->Transform()->translate.Set(x, y, 0);
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		GameObject* ball = GOSpawn(ID_BALL);
		ball->Transform()->translate = m_ghost->Transform()->translate;
		ball->GetComponent<CompKinematicBody>()->linearVel = m_ghost->Transform()->translate - Vector3(x, y, 0);
		float size = Math::Clamp((ball->GetComponent<CompKinematicBody>()->linearVel).LengthSquared() / 1000, 2.f, 10.f);
		ball->Transform()->scale.Set(size, size, size);
		ball->GetComponent<CompKinematicBody>()->mass = size * size * size;
		m_ghost->ActiveSelf(false);
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		m_ghost->ActiveSelf(true);
		m_ghost->Transform()->translate.Set(x, y, 0);
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		GameObject* ball = GOSpawn(ID_BALL);
		ball->Transform()->translate = m_ghost->Transform()->translate;	
		ball->GetComponent<CompKinematicBody>()->linearVel = m_ghost->Transform()->translate - Vector3(x, y, 0);
		float size = Math::Clamp((ball->GetComponent<CompKinematicBody>()->linearVel).LengthSquared() / 1000, 2.f, 10.f);
		ball->Transform()->scale.Set(size, size, size);
		ball->GetComponent<CompKinematicBody>()->mass = size * size * size;
		m_ghost->ActiveSelf(false);
	}
	//bunced input
	if (m_bounceInput < m_elapsedTime)
	{
		//m_bounceInput = m_elapsedTime + 1.f;
	}

	//Physics Simulation Section
	for (int i = 0; i < POOL_SIZE[ID_BALL]; ++i)
	{
		GameObject* go = m_goList[ID_BALL][i];
		if (!(go->IsActive()))
			break;

		HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt * m_speed);
		HelperPhysics::KeepInBounds(go->Transform(), go->GetComponent<CompKinematicBody>(), 0, 0, m_worldWidth, m_worldHeight, 0);
		HelperPhysics::DespawnOffScreen(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 1);
	}

	CheckCollision(dt);	

	SceneBase::PostUpdate(dt);
}

void SceneCollision::GORender(GameObject* go, MgrGraphics::SHADER shader, bool outline)
{
	CompTransform* t = go->Transform();
	if (!(go->IsActive()))
		return;

	m_modelStack.PushMatrix();
	m_modelStack.Translate(t->translate.x, t->translate.y, t->translate.z);
	m_modelStack.Rotate(Math::RadianToDegree(atan2f(t->dir.y, t->dir.x)), 0, 0, 1);
	m_modelStack.Scale(t->scale.x, t->scale.y, t->scale.z);
	RenderMeshGO(go, shader, outline);
	m_modelStack.PopMatrix();
}

void SceneCollision::CheckCollision(double dt)
{
	for (int i = 0; i < POOL_SIZE[ID_BALL]; ++i)
	{
		GameObject* go = m_goList[ID_BALL][i];
		if (!go->IsActive())
			continue;

		for (int j = i + 1; j < POOL_SIZE[ID_BALL]; ++j)
		{
			GameObject* other = m_goList[ID_BALL][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallBall(go, other, 0, &penetration))
			{
				//CompKinematicBody* kb1 = go->GetComponent<CompKinematicBody>();
				//CompKinematicBody* kb2 = other->GetComponent<CompKinematicBody>();
				//m1 = kb1->mass; m2 = kb2->mass; u1 = kb1->linearVel; u2 = kb2->linearVel; //audit stuff
				HelperPhysics::ResolveCollisionBallBall(go, other, penetration);
				//v1 = kb1->linearVel; v2 = kb2->linearVel; //audit stuff
				//m_timerStarted = false;
			}

			float timeToCollide = HelperPhysics::GetTimeToCollideBallBall(go, other);
			//if (!m_timerStarted)
			//{
			//	m_timerStarted = true;
			//	std::cout << m_timeTaken1 << " " << m_estimatedTime << std::endl;
			//	m_timeTaken1 = 0;
			//	m_estimatedTime = 0;
			//}
			//else if (timeToCollide > 0)
			//{
			//	if (timeToCollide > m_estimatedTime)
			//		m_estimatedTime = timeToCollide;
			//	m_timeTaken1 += dt;
			//}
		}

		for (int j = 0; j < POOL_SIZE[ID_WALL]; ++j)
		{
			GameObject* other = m_goList[ID_WALL][j];
			if (!other->IsActive())
				continue;

			Vector3 penetration;
			if (HelperPhysics::IsOverlappingBallWall(go, other, 0, &penetration))
			{
				HelperPhysics::ResolveCollisionBallWall(go, other, penetration);
				m_timerStarted = false;
			}

			float timeToCollide = HelperPhysics::GetTimeToCollideBallWall(go, other);
			if (!m_timerStarted)
			{
				m_timerStarted = true;
				std::cout << m_timeTaken1 << " " << m_estimatedTime << std::endl;
				m_timeTaken1 = 0;
				m_estimatedTime = 0;
			}
			else if (timeToCollide > 0)
			{
				if (timeToCollide > m_estimatedTime)
					m_estimatedTime = timeToCollide;
				m_timeTaken1 += dt;
			}
		}
	}
}

void SceneCollision::Render()
{
	SceneBase::PreRender();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	GORender(m_ghost, MGR_GRAPHICS.LIT);

	for (auto pool : m_goList)
	{
		for (auto go : pool.second)
		{
			GORender(go, MGR_GRAPHICS.SIMPLE);
		}
	}

	//Exercise 8c: Render initial and final momentum

	//On screen text
	std::ostringstream ss;

	ss.precision(5); ss.str(""); ss.clear();
	ss << "Audit: ";	
	//ss << "Bef M:" << (m1 * u1 + m2 * u2).Length() << " | Aft M: " << (m1 * u1 + m2 * u2).Length();
	//ss << " | Bef K:" << 0.5f * m1 * u1.Dot(u1) + 0.5f * m2 * u2.Dot(u2) << " | Aft K: " << 0.5f * m1 * v1.Dot(v1) + 0.5f * m2 * v2.Dot(v2);
	ss << m_timeTaken1 << " " << m_estimatedTime << "  See result in console";
	RenderTextOnScreen(ss.str(), 1.f, 9.5f);

	ss.precision(5); ss.str(""); ss.clear();
	ss << "GameObject Count: " << GameObject::count;
	RenderTextOnScreen(ss.str(), 1.f, 6.5f);

	ss.precision(5); ss.str(""); ss.clear();
	ss << "Simul Speed: " << m_speed << " | FPS: " << m_fps;
	RenderTextOnScreen(ss.str(), 1.f, 3.5f);
	
	RenderTextOnScreen("Collision", 1.f, 0.5f);

	SceneBase::PostRender();
}

void SceneCollision::Exit()
{
	//Cleanup GameObjects
	for (auto pool : m_goList)
	{
		for (auto go : pool.second)
		{
			delete go;
			go = nullptr;
		}
	}

	delete m_ghost;
	m_ghost = nullptr;
	SceneBase::Exit();
}
