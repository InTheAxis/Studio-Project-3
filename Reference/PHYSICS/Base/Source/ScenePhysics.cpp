#include "ScenePhysics.h"
#include "Application.h"

ScenePhysics::ScenePhysics()
{
}

ScenePhysics::~ScenePhysics()
{
}

void ScenePhysics::Init()
{
	SceneBase::Init();

	GOInit();
	InitMiscMeshes();

	for (int i = 0; i < BOUNCE_TOTAL; ++i)
		m_bounceTimes[i] = m_elapsedTime + BOUNCE_DELAY[i];

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

	gameState = GS_MENU;

	GOSpawn(GO_BG);
	player = GOSpawn(GO_SHIP);

	Reset();
}

void ScenePhysics::Update(double dt)
{
	SceneBase::PreUpdate(dt);

	if (gameState == GS_GAME)
	{
		BouncedUpdate(dt);
		QuickUpdate(dt);
	}
	
	if (m_bounceTimes[BOUNCE_INPUT] < m_elapsedTime)
	{
		m_bounceTimes[BOUNCE_INPUT] = m_elapsedTime + BOUNCE_DELAY[BOUNCE_INPUT];
		if (gameState == GS_MENU && Application::IsKeyPressed(VK_SPACE))
		{
			gameState = GS_GAME;
		}
		else if (gameState == GS_LOSE && Application::IsKeyPressed(VK_SPACE))
		{
			gameState = GS_MENU;
		}
	}

	SceneBase::PostUpdate(dt);
}

void ScenePhysics::Render()
{
	SceneBase::PreRender();

	if (gameState == GS_MENU)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
		m_modelStack.Scale(m_worldHeight, m_worldHeight, 1);
		MGR_GRAPHICS.RenderMesh(miscMeshList[MM_MENU], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();		 
		RenderTextOnScreen("Press <SPACE> to Start", 55, 3);
	}
	if (gameState == GS_LOSE)
	{
		m_modelStack.PushMatrix();
		m_modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
		m_modelStack.Scale(m_worldHeight, m_worldHeight, 1);
		MGR_GRAPHICS.RenderMesh(miscMeshList[MM_LOSE], MGR_GRAPHICS.UNLIT);
		m_modelStack.PopMatrix();
		RenderTextOnScreen("Press <SPACE> to go back", 55, 3);
	}

	if (gameState == GS_GAME)
	{
		//render each gameobject pool
		for (int i = 0; i < GO_TOTAL; ++i)
		{
			GORenderPool((GAMEOBJECT_ID)i, MGR_GRAPHICS.UNLIT);
		}

		//On screen text
		std::ostringstream ss;
		int w = Application::GetWindowWidth() * 0.1;
		int h = Application::GetWindowHeight() * 0.1 - 12;

		ss.str("");
		ss.precision(4);
		ss << "FPS: " << m_fps;
		RenderTextOnScreen(ss.str(), w - 23, h);

		ss.str("");
		ss.precision(4);
		ss << "GameObjects : " << GameObject::count;
		RenderTextOnScreen(ss.str(), 1, 3);

		ss.str("");
		ss.precision(4);
		ss << "Enemies : " << m_numEnemies;
		RenderTextOnScreen(ss.str(), 1, 6);

		ss.str("");
		ss.precision(5);
		ss << "Lives: " << m_lives << " | HP: " << player->GetComponent<CompHealth>()->hp;
		RenderTextOnScreen(ss.str(), 1, h);

		ss.str("");
		ss.precision(5);
		ss << "Score: " << m_score;
		RenderTextOnScreen(ss.str(), 1, h - 3);

		ss.str("");
		ss.precision(5);
		ss << "Missles: " << player->GetComponent<CompAmmo>()->ammo[AMMO_MISSLE] << "/" << player->GetComponent<CompAmmo>()->MAX_AMMO[AMMO_MISSLE];
		RenderTextOnScreen(ss.str(), 1, h - 6);

	}
	SceneBase::PostRender();
}

void ScenePhysics::Exit()
{
	for (unsigned i = 0; i < GO_TOTAL; ++i)
	{
		for (unsigned j = 0; j < POOL_SIZE[i]; ++j)
		{
			if (m_goList[(GAMEOBJECT_ID)i][j])
				delete m_goList[(GAMEOBJECT_ID)i][j];
		}
	}

	SceneBase::Exit();
}

void ScenePhysics::Reset()
{
	for (unsigned i = 0; i < GO_TOTAL; ++i)
	{	
		if (i == GO_BG || i == GO_SHIP)
			continue;
		for (unsigned j = 0; j < POOL_SIZE[i]; ++j)
		{
			m_goList[(GAMEOBJECT_ID)i][j]->ActiveSelf(false);
		}
	}

	PlayerReset();
	m_score = 0;
	m_lives = MAX_LIVES;
	m_numEnemies = 0;

	if (gameState == GS_GAME)
		gameState = GS_LOSE;
}

void ScenePhysics::InitMiscMeshes()
{
	miscMeshList[MM_HPBAR] = MeshBuilder::GenerateQuad("hpbar", Color(0.5f, 0.5f, 0.5f), 1);
	miscMeshList[MM_HPBAR]->color.Set(0, 1, 0);
	miscMeshList[MM_TARGET] = MeshBuilder::GenerateQuad("mark", Color(0.5f, 0.5f, 0.5f), 1);
	miscMeshList[MM_TARGET]->textureArray[0] = LoadTGA("Image//Physics//targetMark.tga");
	miscMeshList[MM_MENU] = MeshBuilder::GenerateQuad("menu", Color(0.5f, 0.5f, 0.5f), 1);
	miscMeshList[MM_MENU]->textureArray[0] = LoadTGA("Image//Physics//menu.tga");
	miscMeshList[MM_LOSE] = MeshBuilder::GenerateQuad("menu", Color(0.5f, 0.5f, 0.5f), 1);
	miscMeshList[MM_LOSE]->textureArray[0] = LoadTGA("Image//Physics//oopsie.tga");
}

void ScenePhysics::GOInit()
{
	//create meshes
	meshList[GO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1, 32);
	meshList[GO_BG]->textureArray[0] = LoadTGA("Image//Physics//space.tga");
	meshList[GO_SHIP] = MeshBuilder::GenerateTriangle("ship", Color(1, 1, 1));
	meshList[GO_SHIP]->textureArray[0] = LoadTGA("Image//Physics//pewdPattern.tga");
	meshList[GO_ASTEROID] = MeshBuilder::GenerateSphere("ast", Color(1, 1, 1), 8, 8);
	meshList[GO_ASTEROID]->textureArray[0] = LoadTGA("Image//Physics//rock.tga");
	meshList[GO_ASTEROID]->textureArray[1] = LoadTGA("Image//Physics//spaceMoss.tga");
	meshList[GO_ENEMY_KAMIKAZE] = MeshBuilder::GenerateQuad("kamikaze enemy", Color(1, 1, 1));
	meshList[GO_ENEMY_KAMIKAZE]->textureArray[0] = LoadTGA("Image//Physics//tSeries.tga");
	meshList[GO_ENEMY_SHOOTING] = MeshBuilder::GenerateQuad("shooting enemy", Color(1, 1, 1));
	meshList[GO_ENEMY_SHOOTING]->textureArray[0] = LoadTGA("Image//Physics//notyoutube.tga");
	meshList[GO_LMG_ENEMY] = MeshBuilder::GenerateQuad("lmg enemy", Color(1, 1, 0));
	meshList[GO_LMG_ENEMY]->textureArray[0] = LoadTGA("Image//Physics//demonetize.tga");
	meshList[GO_LMG_BULLET] = MeshBuilder::GenerateQuad("lmg", Color(1, 1, 0));
	meshList[GO_LMG_BULLET]->textureArray[0] = LoadTGA("Image//Physics//monetize.tga");
	meshList[GO_MISSLE] = MeshBuilder::GenerateQuad("missle", Color(1, 1, 0));
	meshList[GO_MISSLE]->textureArray[0] = LoadTGA("Image//Physics//musicNote.tga");
	meshList[GO_BLACKHOLE] = MeshBuilder::GenerateQuad("blackhole", Color(1, 1, 0));
	meshList[GO_BLACKHOLE]->textureArray[0] = LoadTGA("Image//Physics//blackhole.tga");
	meshList[GO_REPULSOR] = MeshBuilder::GenerateQuad("repulsor", Color(1, 1, 0));
	meshList[GO_REPULSOR]->textureArray[0] = LoadTGA("Image//Physics//repulsor.tga");
	meshList[GO_BUDDY_HEAL] = MeshBuilder::GenerateQuad("buddy heal", Color(0, 1, 0), 1, 1);
	meshList[GO_BUDDY_HEAL]->textureArray[0] = LoadTGA("Image//Physics//buddyHeal.tga");
	meshList[GO_BUDDY_KAMIKAZE] = MeshBuilder::GenerateQuad("buddy kamikaze", Color(0, 1, 0), 1, 1);
	meshList[GO_BUDDY_KAMIKAZE]->textureArray[0] = LoadTGA("Image//Physics//blueshirt.tga");
	meshList[GO_POWERUP] = MeshBuilder::GenerateQuad("powerup", Color(0, 1, 0), 1, 1); //temporary
	meshList[GO_POWERUP]->textureArray[0] = LoadTGA("Image//Physics//powerup.tga");

	//create gameobjects
	for (unsigned i = 0; i < GO_TOTAL; ++i)
	{
		m_goList[(GAMEOBJECT_ID)i].resize(POOL_SIZE[i]);
		for (unsigned j = 0; j < POOL_SIZE[i]; ++j)
		{
			m_goList[(GAMEOBJECT_ID)i][j] = new GameObject;				
		}		
	}
}

GameObject* ScenePhysics::GOSpawn(GAMEOBJECT_ID id)
{
	GameObject* go = GOFetch(id);
	go->ActiveSelf(true);
	go->AddComponent<CompMesh>(meshList[id]);	
	go->SetTag(static_cast<unsigned long long>(id));
	//cache components
	CompTransform* t = go->Transform();	
	CompKinematicBody* kb;	
	CompHealth* hea;
	CompTracker* track;
	CompGravitation* grav;
	CompTimer* timer;
	CompPowerup* pwup;
	CompAmmo* amm;
	switch (id)
	{
	case GO_BG:
		t->translate.Set(500, 500, -10);
		t->scale.Set(1000, 1000, -1);
		break;
	case GO_SHIP:
		go->AddComponent<CompKinematicBody>();
		go->AddComponent<CompHealth>();
		go->AddComponent<CompTimer>();//for tracking firerates
		go->AddComponent<CompTracker>(); //for tracking home'd targets
		go->AddComponent<CompAmmo>(); //for tracking ammo use
		//init done in PlayerReset();
		break;
	case GO_ASTEROID:		
		t->translate.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
		
		kb = go->AddComponent<CompKinematicBody>();		
		kb->FORCE_MAG = 100;
		kb->TORQUE_MAG = 50;	
		kb->MAX_LIN_VEL = 5;
		kb->MAX_ANG_VEL = 10;
		kb->mass = static_cast<float>(Math::RandIntMinMax(2, 4));
		go->Transform()->scale.Set(static_cast<float>(kb->mass), static_cast<float>(kb->mass), 1);
		kb->momentOfInertia = kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->force.Set(Math::RandFloatMinMax(-kb->FORCE_MAG, kb->FORCE_MAG), Math::RandFloatMinMax(-kb->FORCE_MAG, kb->FORCE_MAG), 0);
		kb->torque = (Vector3(0, go->Transform()->scale.y, 0).Cross(Vector3(Math::RandFloatMinMax(-kb->TORQUE_MAG, kb->TORQUE_MAG), 0, 0))).z;		
		break;
	case GO_ENEMY_KAMIKAZE:
		t->translate.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);		
		t->dir = (player->Transform()->translate - t->translate).Normalized();
		t->scale.Set(5, 5, 1);

		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 1;
		kb->momentOfInertia = 0.5f * kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->FORCE_MAG = 100;
		kb->TORQUE_MAG = 50;
		kb->MAX_LIN_VEL = 20;
		kb->MAX_ANG_VEL = 2;
		hea = go->AddComponent<CompHealth>();
		hea->hp = hea->MAX_HP = 2;

		m_numEnemies++;
		break;		
	case GO_ENEMY_SHOOTING:
		t->translate.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
		t->dir = (player->Transform()->translate - t->translate).Normalized();
		t->scale.Set(5, 5, 1);

		go->AddComponent<CompTimer>()->targetTime[0] = 2.5f; //lmg firerate
		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 1;
		kb->momentOfInertia = 0.5f * kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->FORCE_MAG = 50;
		kb->TORQUE_MAG = 50;
		kb->MAX_LIN_VEL = 20;
		kb->MAX_ANG_VEL = 2;
		hea = go->AddComponent<CompHealth>();
		hea->hp = hea->MAX_HP = 5;

		m_numEnemies++;
		break;
	case GO_LMG_ENEMY:
		kb = go->AddComponent<CompKinematicBody>();
		kb->MAX_LIN_VEL = 80;
		break;
	case GO_LMG_BULLET:
		t->translate = player->Transform()->translate + player->Transform()->dir * player->Transform()->scale.y * 0.5f;
		t->translate.z = 1;
		t->dir = player->Transform()->dir;
		t->scale.Set(2.f, 2.f, 1);

		kb = go->AddComponent<CompKinematicBody>();
		kb->MAX_LIN_VEL = 80;
		kb->linearVel = go->Transform()->dir * kb->MAX_LIN_VEL;
		break;
	case GO_MISSLE:
		amm = player->GetComponent<CompAmmo>();
		amm->ammo[AMMO_MISSLE] = Math::Max(0, amm->ammo[AMMO_MISSLE] - 1);
		t->translate = player->Transform()->translate + player->Transform()->dir * player->Transform()->scale.y * 0.5f;
		t->translate.z = 1;
		t->dir = player->Transform()->dir;
		t->scale.Set(5.f, 5.f, 1);

		kb = go->AddComponent<CompKinematicBody>();		
		kb->mass = 1;
		kb->momentOfInertia = 0.5f * kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->FORCE_MAG = 100;
		kb->TORQUE_MAG = 40;
		kb->MAX_LIN_VEL = 50;
		kb->MAX_ANG_VEL = 2;
		

		track = player->GetComponent<CompTracker>();
		if (track->target == nullptr)
		{
			if (Math::RandIntMinMax(0, 1))
			{
				for (unsigned i = 0; i < POOL_SIZE[GO_ENEMY_KAMIKAZE]; ++i)
				{
					GameObject* other = m_goList[GO_ENEMY_KAMIKAZE][i];
					if (other->IsActive())
					{
						track->target = other;
						break;
					}
				}
			}
			else
			{
				for (unsigned i = 0; i < POOL_SIZE[GO_ENEMY_SHOOTING]; ++i)
				{
					GameObject* other = m_goList[GO_ENEMY_SHOOTING][i];
					if (other->IsActive())
					{
						track->target = other;
						break;
					}
				}
			}
		}
		break;
	case GO_BLACKHOLE:
		t->translate.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 5);
		t->dir.Set(0, 1, 0);
		t->scale.Set(1.f, 1.f, 1);
		
		go->AddComponent<CompTimer>()->targetTime[0] = 3;
		grav = go->AddComponent<CompGravitation>();
		grav->MAX_SCALE = Math::RandFloatMinMax(15, 20);
		grav->MAX_TRIGGER_DIST = grav->MAX_SCALE * 3;
		grav->GRAV_CONSTANT = 40;
		break;
	case GO_REPULSOR:
		t->translate = player->Transform()->translate;
		t->translate.z = 4;
		t->dir.Set(0, 1, 0);
		t->scale.Set(20, 20, 1);		

		go->AddComponent<CompTimer>()->targetTime[0] = 5;
		grav = go->AddComponent<CompGravitation>();
		grav->MAX_SCALE = 10;
		grav->MAX_TRIGGER_DIST = grav->MAX_SCALE * 2;
		grav->GRAV_CONSTANT = 400;
		break;
	case GO_BUDDY_HEAL:
		t->translate = player->Transform()->translate - player->Transform()->dir * 10;
		t->dir = (player->Transform()->translate - t->translate).Normalized();
		t->scale.Set(5, 5, 1);

		go->AddComponent<CompTimer>()->targetTime[0] = 10; //amount of time it lasts
		go->AddComponent<CompTimer>()->targetTime[1] = 0.3; //reaction delay

		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 0.5f;
		kb->momentOfInertia = kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->FORCE_MAG = 40;
		kb->TORQUE_MAG = 50;
		kb->MAX_LIN_VEL = 20;
		kb->MAX_ANG_VEL = 2;
		break;	
	case GO_BUDDY_KAMIKAZE:
		t->translate = player->Transform()->translate - player->Transform()->dir * 10;
		t->dir = (player->Transform()->translate - t->translate).Normalized();
		t->scale.Set(5, 5, 1);

		kb = go->AddComponent<CompKinematicBody>();
		kb->mass = 0.5f;
		kb->momentOfInertia = kb->mass * go->Transform()->scale.x * go->Transform()->scale.y;
		kb->FORCE_MAG = 50;
		kb->TORQUE_MAG = 100;
		kb->MAX_LIN_VEL = 10;
		kb->MAX_ANG_VEL = 2;
		break;
	case GO_POWERUP:
		t->translate.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);		
		t->scale.Set(3, 3, 1);

		timer = go->AddComponent<CompTimer>();
		pwup = go->AddComponent<CompPowerup>();
		
		pwup->type = (CompPowerup::POWERUP_TYPE)(Math::RandIntMinMax(0, pwup->PWUP_TOTAL - 1));

		switch (pwup->type)
		{
		case pwup->PWUP_MISSLE_REFILL:			
			timer->targetTime[0] = 0.0; //amount of time it lasts
			break;
		case pwup->PWUP_BUDDY_HEAL:			
			timer->targetTime[0] = 0.0; //amount of time it lasts
			break;
		case pwup->PWUP_BUDDY_KAMIKAZE:
			timer->targetTime[0] = 0.0; //amount of time it lasts
			break;
		case pwup->PWUP_BARRIER:
			timer->targetTime[0] = 0.0; //amount of time it lasts
			break;
		}

		break;
	}

	return go;
}

GameObject* ScenePhysics::GOFetch(GAMEOBJECT_ID id)
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

void ScenePhysics::GORenderPool(GAMEOBJECT_ID id, MgrGraphics::SHADER shader, bool outline)
{
	for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
	{
		GameObject* go = m_goList[id][i];
		CompTransform* t = go->Transform();
		float degree;
		if (!(go->IsActive()))
			continue;
		m_modelStack.PushMatrix();
		m_modelStack.Translate(t->translate.x, t->translate.y, t->translate.z);		
		if (!(t->dir.IsZero()))
		{
			degree = Math::RadianToDegree(atanf(t->dir.y / t->dir.x));
			if (t->dir.x < 0)
				degree += 180.f;
			m_modelStack.Rotate(degree - 90, 0, 0, 1);
		}
		m_modelStack.Scale(t->scale.x, t->scale.y, t->scale.z);
		RenderMeshGO(go, shader, outline);
		m_modelStack.PopMatrix();	

		m_modelStack.PushMatrix();
		m_modelStack.Translate(t->translate.x, t->translate.y, t->translate.z);
		/*Special stuff*/
		CompHealth* hea = go->GetComponent<CompHealth>();
		if (hea)
		{
			//the actual bar
			float percent = (float)hea->hp / hea->MAX_HP;
			m_modelStack.PushMatrix();
			m_modelStack.Translate((percent - 1) * 3, -t->scale.y, 7);
			m_modelStack.Scale(percent * 6, 1, 1);
			miscMeshList[MM_HPBAR]->color.Set(0, 0.8f, 0);
			MGR_GRAPHICS.RenderMesh(miscMeshList[MM_HPBAR], MGR_GRAPHICS.SIMPLE);
			m_modelStack.PopMatrix();
			//bg
			m_modelStack.PushMatrix();
			m_modelStack.Translate(0, -t->scale.y, 6);
			m_modelStack.Scale(6, 0.5f, 1);
			miscMeshList[MM_HPBAR]->color.Set(0.8f, 0, 0);
			MGR_GRAPHICS.RenderMesh(miscMeshList[MM_HPBAR], MGR_GRAPHICS.SIMPLE);
			m_modelStack.PopMatrix();
		}
		if (go == player->GetComponent<CompTracker>()->target)
		{
			m_modelStack.PushMatrix();
			m_modelStack.Translate(0, 3, 2);
			m_modelStack.Scale(6, 6, 1);
			MGR_GRAPHICS.RenderMesh(miscMeshList[MM_TARGET], MGR_GRAPHICS.UNLIT);
			m_modelStack.PopMatrix();
		}
		m_modelStack.PopMatrix();
	}
}

void ScenePhysics::BouncedUpdate(double dt)
{
	for (int i = 0; i < BOUNCE_TOTAL; ++i)
	{
		if (m_bounceTimes[i] < m_elapsedTime)
		{
			m_bounceTimes[i] = m_elapsedTime + BOUNCE_DELAY[i];
			switch ((BOUNCE_TIMES)i)
			{
			case BOUNCE_COLLISION:				
				//handle in order, first come first serve
				for (int i = GO_SHIP + 1; i < GO_TOTAL; ++i)
				{
					GOCheckCollision((GAMEOBJECT_ID)i);
				}
				break;
			case BOUNCE_SPAWN_AST:				
				for (int i = 0; i < Math::RandIntMinMax(1, 4); ++i)
				{
					GOSpawn(GO_ASTEROID);
				}
				break;
			case BOUNCE_SPAWN_ENEMY:				
				for (int i = 0; i < Math::RandIntMinMax(1, 4); ++i)
				{				
					if (Math::RandIntMinMax(0, 1))
						GOSpawn(GO_ENEMY_KAMIKAZE);
					else
						GOSpawn(GO_ENEMY_SHOOTING);
				}
				break;
			case BOUNCE_SPAWN_BLACKHOLE:
				GOSpawn(GO_BLACKHOLE);
				break;
			case BOUNCE_SPAWN_POWERUP:
				GOSpawn(GO_POWERUP);
				break;
			}
		}
	}
}

void ScenePhysics::QuickUpdate(double dt)
{
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		CompKinematicBody* kb = player->GetComponent<CompKinematicBody>();
		kb->mass += 0.1f;
		kb->momentOfInertia = 0.5f * kb->mass * player->Transform()->scale.x * player->Transform()->scale.y;
	}
	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		CompKinematicBody* kb = player->GetComponent<CompKinematicBody>();
		kb->mass = Math::Max(0.1f, kb->mass - 0.1f);
		kb->momentOfInertia = 0.5f * kb->mass * player->Transform()->scale.x * player->Transform()->scale.y;
	}
	if (Application::IsKeyPressed('V'))
		GOSpawn(GO_ASTEROID);
	if (Application::IsKeyPressed('B'))
		GOSpawn(GO_BLACKHOLE);
	if (Application::IsKeyPressed('N'))
		GOSpawn(GO_ENEMY_KAMIKAZE);
	if (Application::IsKeyPressed('M'))
		GOSpawn(GO_ENEMY_SHOOTING);
	if (Application::IsKeyPressed('R'))
	{
		GOSpawn(GO_POWERUP);
	}
	if (Application::IsKeyPressed('F') && !m_goList[GO_BUDDY_HEAL][0]->IsActive())
	{
		GOSpawn(GO_BUDDY_HEAL);			
	}
	if (Application::IsKeyPressed('G') && !m_goList[GO_BUDDY_KAMIKAZE][0]->IsActive())
	{
		GOSpawn(GO_BUDDY_KAMIKAZE);		
	}
	if (Application::IsKeyPressed('H'))
	{
		GameObject* pw = GOSpawn(GO_POWERUP);
		pw->GetComponent<CompPowerup>()->type = pw->GetComponent<CompPowerup>()->PWUP_MISSLE_REFILL;
	}
	if (Application::IsKeyPressed('J') && !m_goList[GO_REPULSOR][0]->IsActive())
	{
		GOSpawn(GO_REPULSOR);
	}

	//controls
	PlayerController(dt);

	//update gameobjects
	for (int i = GO_SHIP; i < GO_TOTAL; ++i)
	{
		GOUpdate((GAMEOBJECT_ID)i, dt);
	}

	//reset condition
	if (m_lives <= 0)
		Reset();
}

void ScenePhysics::GOUpdate(GAMEOBJECT_ID id, double dt)
{
	//cache
	GameObject* go;	
	CompHealth* hea;
	CompKinematicBody* kb;
	CompTracker* track;
	CompGravitation* grav;
	CompTimer* timer;
	switch (id)
	{
	case GO_SHIP:	
		//kinematics
		HelperPhysics::UpdateKinematics(player->GetComponent<CompKinematicBody>(), player->Transform(), dt);
		//wrap around
		HelperPhysics::WrapAround(player->Transform(), 0, 0, m_worldWidth, m_worldHeight, -3);
		timer = player->GetComponent<CompTimer>();
		timer->deltaTime[0] += dt; //mg
		timer->deltaTime[1] += dt; //missle
		hea = player->GetComponent<CompHealth>();
		if (hea->hp <= 0)
		{
			m_lives--;
			hea->hp = hea->MAX_HP;
		}
		break;	
	case GO_ASTEROID:	
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			//kinematics
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//wrap around
			HelperPhysics::WrapAround(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 0);
		}
		break;
	case GO_ENEMY_KAMIKAZE:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			if (go->GetComponent<CompHealth>()->hp <= 0)
			{
				go->ActiveSelf(false);
				m_numEnemies = Math::Max(0, m_numEnemies - 1);
				continue;
			}
			kb = go->GetComponent<CompKinematicBody>();
			
			kb->force = go->Transform()->dir * kb->FORCE_MAG;
			kb->linearVel = go->Transform()->dir * kb->MAX_LIN_VEL; //give it some intial vel
			Vector3 intendedDir = (player->Transform()->translate - go->Transform()->translate).Normalized();
			go->Transform()->dir = HelperPhysics::Lerp(go->Transform()->dir, intendedDir, 0.1f);

			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//wrap around
			HelperPhysics::WrapAround(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, -4);
		}
		break;
	case GO_ENEMY_SHOOTING:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			if (go->GetComponent<CompHealth>()->hp <= 0)
			{
				go->ActiveSelf(false);
				m_numEnemies = Math::Max(0, m_numEnemies - 1);
				continue;
			}
			kb = go->GetComponent<CompKinematicBody>();
				
			Vector3 intendedDir = (player->Transform()->translate - go->Transform()->translate);
			if (intendedDir.LengthSquared() < 400)
			{
				kb->force += go->Transform()->dir * -kb->FORCE_MAG;
			}
			go->Transform()->dir = HelperPhysics::Lerp(go->Transform()->dir, intendedDir.Normalized(), 0.1f);

			GameObject* bullet;
			timer = go->GetComponent<CompTimer>();
			timer->deltaTime[0] += dt;

			if (timer->deltaTime[0] > timer->targetTime[0])
			{
				bullet = GOSpawn(GO_LMG_ENEMY);
				timer->deltaTime[0] = 0.0;
				CompTransform* t = bullet->Transform();
				t->translate = go->Transform()->translate + go->Transform()->dir * go->Transform()->scale.y * 0.5f;
				t->translate.z = 1;
				t->dir = go->Transform()->dir;
				t->scale.Set(2.f, 2.f, 1);
				bullet->GetComponent<CompKinematicBody>()->linearVel = go->Transform()->dir * kb->MAX_LIN_VEL;
			}

			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//wrap around
			HelperPhysics::WrapAround(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, -5);
		}
		break;
	case GO_LMG_ENEMY:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//despawn
			HelperPhysics::DespawnOffScreen(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 0);
		}
		break;
	case GO_LMG_BULLET:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//despawn
			HelperPhysics::DespawnOffScreen(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 0);
		}
		break;
	case GO_MISSLE:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			//finding target
			track = player->GetComponent<CompTracker>();
			kb = go->GetComponent<CompKinematicBody>();

			kb->force = go->Transform()->dir * kb->FORCE_MAG;
			kb->linearVel = go->Transform()->dir * kb->MAX_LIN_VEL; //give it some intial vel
			if (track->target != nullptr)			
			{
				Vector3 intendedDir = (track->target->Transform()->translate - go->Transform()->translate).Normalized();
				go->Transform()->dir = HelperPhysics::Lerp(go->Transform()->dir, intendedDir, 0.1f);
			}
			//kinematics	
			HelperPhysics::UpdateKinematics(kb, go->Transform(), dt);
			//despawn
			HelperPhysics::DespawnOffScreen(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, 0);
		}
		break;
	case GO_BLACKHOLE:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;

			GameObject* other;
			grav = go->GetComponent<CompGravitation>();
			timer = go->GetComponent<CompTimer>();
			timer->deltaTime[0] += dt;
			
			if (timer->deltaTime[0] > timer->targetTime[0])
			{
				go->Transform()->scale = HelperPhysics::Lerp(go->Transform()->scale, Vector3(0, 0, 1), dt * 2);
				if (go->Transform()->scale.x < 2)
				{
					timer->deltaTime[0] = 0;
					go->ActiveSelf(false);
					continue;
				}
			}
			else
				go->Transform()->scale = HelperPhysics::Lerp(go->Transform()->scale, Vector3(grav->MAX_SCALE, grav->MAX_SCALE, 1), dt);		

			go->Transform()->dir.Set(cosf(timer->deltaTime[0]), sinf(timer->deltaTime[0]), 0);
			//affect all selected kinematic bodies
			for (unsigned j = GO_SHIP; j < GO_TOTAL; ++j)
			{
				if (id == j) continue;
				for (unsigned k = 0; k < POOL_SIZE[j]; ++k)
				{
					other = m_goList[(GAMEOBJECT_ID)j][k];
					if (!other->IsActive())
						continue;
					kb = other->GetComponent<CompKinematicBody>();
					if (!kb)
						continue;
					Vector3 dir = (go->Transform()->translate - other->Transform()->translate);
					if (dir.LengthSquared() < grav->MAX_TRIGGER_DIST * grav->MAX_TRIGGER_DIST)
						kb->force += dir.Normalized() * (grav->GRAV_CONSTANT * go->Transform()->scale.x * kb->mass / (other->Transform()->scale.x * other->Transform()->scale.y));
				}
			}
		}
		break;
	case GO_REPULSOR:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			
			GameObject* other;
			grav = go->GetComponent<CompGravitation>();
			timer = go->GetComponent<CompTimer>();
			timer->deltaTime[0] += dt;

			if (timer->deltaTime[0] > timer->targetTime[0])
			{
				go->Transform()->scale = HelperPhysics::Lerp(go->Transform()->scale, Vector3(0, 0, 1), dt * 2);
				if (go->Transform()->scale.x < 2)
				{
					timer->deltaTime[0] = 0;
					go->ActiveSelf(false);
					continue;
				}
			}
			else
				go->Transform()->scale = HelperPhysics::Lerp(go->Transform()->scale, Vector3(grav->MAX_SCALE, grav->MAX_SCALE, 1), dt);

			go->Transform()->translate = player->Transform()->translate;	
			go->Transform()->dir.Set(cosf(timer->deltaTime[0]), sinf(timer->deltaTime[0]), 0);
			//affect all selected kinematic bodies
			//affect all selected kinematic bodies
			for (unsigned j = GO_SHIP + 1; j < GO_TOTAL; ++j)
			{
				if (id == j) continue;
				for (unsigned k = 0; k < POOL_SIZE[j]; ++k)
				{
					other = m_goList[(GAMEOBJECT_ID)j][k];
					if (!other->IsActive())
						continue;
					kb = other->GetComponent<CompKinematicBody>();
					if (!kb)
						continue;
					Vector3 dir = (go->Transform()->translate - other->Transform()->translate);
					if (dir.LengthSquared() < grav->MAX_TRIGGER_DIST * grav->MAX_TRIGGER_DIST)
						kb->force -= dir.Normalized() * (grav->GRAV_CONSTANT * go->Transform()->scale.x * kb->mass / (other->Transform()->scale.x * other->Transform()->scale.y));
				}
			}
		}
		break;
	case GO_BUDDY_HEAL:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;

			timer = go->GetComponent<CompTimer>();
			kb = go->GetComponent<CompKinematicBody>();

			timer->deltaTime[0] += dt;
			timer->deltaTime[1] += dt;

			if (timer->deltaTime[0] > timer->targetTime[0])
			{
				go->ActiveSelf(false);
				timer->deltaTime[0] = 0.0;
				continue;
			}
				
			Vector3 intendedDir = (player->Transform()->translate - go->Transform()->translate);
			go->Transform()->dir = intendedDir.Normalized();

			if (timer->deltaTime[1] > timer->targetTime[1])
			{
				if (intendedDir.LengthSquared() > (go->Transform()->scale.x + player->Transform()->scale.x) * (go->Transform()->scale.x + player->Transform()->scale.x))
				{
					go->Transform()->dir = HelperPhysics::Lerp(go->Transform()->dir, intendedDir.Normalized(), 0.1f);
					kb->force = go->Transform()->dir * kb->FORCE_MAG;
				}
				else //within rnage of player
				{
					timer->deltaTime[1] = 0.0;
					CompHealth* hea = player->GetComponent<CompHealth>();
					hea->hp = Math::Min(hea->MAX_HP, hea->hp + 5);
				}
			}
			else
			{
				kb->linearVel = HelperPhysics::Lerp(kb->linearVel, 0, 0.2f);
			}

			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//wrap around
			HelperPhysics::WrapAround(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, -3);
		}
		break;
	case GO_BUDDY_KAMIKAZE:
		for (unsigned i = 0; i < POOL_SIZE[id]; ++i)
		{
			go = m_goList[id][i];
			if (!go->IsActive())
				continue;
			
			kb = go->GetComponent<CompKinematicBody>();

			//kinematics	
			HelperPhysics::UpdateKinematics(go->GetComponent<CompKinematicBody>(), go->Transform(), dt);
			//wrap around
			HelperPhysics::WrapAround(go->Transform(), 0, 0, m_worldWidth, m_worldHeight, -3);
		}
		break;
	}
}

void ScenePhysics::GOCheckCollision(GAMEOBJECT_ID id)
{	
	//store size first so that spawning new obj doesn't screw up the loop
	unsigned size = POOL_SIZE[id];	
	switch (id)
	{
		case GO_ASTEROID: //with player, lmg, lmg enemy missle
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_ASTEROID][j];
				if (!go->IsActive())
					continue;

				//asteroid with player
				if (HelperPhysics::IsColliding(go, player))
				{
					player->GetComponent<CompHealth>()->hp -= 2;
					player->GetComponent<CompKinematicBody>()->force = (player->Transform()->translate - go->Transform()->translate) * player->GetComponent<CompKinematicBody>()->FORCE_MAG;
					//papa asteroid
					if (go->Transform()->scale.x >= 3.f) //limit
					{
						GameObject* baby;
						CompKinematicBody* kb;
						for (int i = 0; i < Math::RandIntMinMax(2, 3); ++i)
						{
							baby = GOSpawn(GO_ASTEROID);
							baby->Transform()->translate = go->Transform()->translate;
							baby->Transform()->scale = go->Transform()->scale - Vector3(1, 1, 1);
							kb = baby->GetComponent<CompKinematicBody>();
							kb->mass = go->GetComponent<CompKinematicBody>()->mass - 1;
							kb->force *= player->GetComponent<CompKinematicBody>()->MAX_LIN_VEL;
						}
					}
					go->ActiveSelf(false);
				}

				//asteroid with lmg bullet		
				for (unsigned k = 0; k < POOL_SIZE[GO_LMG_BULLET]; ++k)
				{
					GameObject* other = m_goList[GO_LMG_BULLET][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score++;
						//papa asteroid
						if (go->Transform()->scale.x >= 3.f) //limit
						{
							GameObject* baby;
							CompKinematicBody* kb;
							for (int i = 0; i < Math::RandIntMinMax(2, 3); ++i)
							{
								baby = GOSpawn(GO_ASTEROID);
								baby->Transform()->translate = go->Transform()->translate;
								baby->Transform()->scale = go->Transform()->scale - Vector3(1, 1, 1);
								kb = baby->GetComponent<CompKinematicBody>();
								kb->mass = go->GetComponent<CompKinematicBody>()->mass - 1;
								kb->force *= other->GetComponent<CompKinematicBody>()->MAX_LIN_VEL;
							}
						}
						go->ActiveSelf(false);
					}
				}

				//asteroid with lmg enemy bullet		
				for (unsigned k = 0; k < POOL_SIZE[GO_LMG_ENEMY]; ++k)
				{
					GameObject* other = m_goList[GO_LMG_ENEMY][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score++;
						//papa asteroid
						if (go->Transform()->scale.x >= 3.f) //limit
						{
							GameObject* baby;
							CompKinematicBody* kb;
							for (int i = 0; i < Math::RandIntMinMax(2, 3); ++i)
							{
								baby = GOSpawn(GO_ASTEROID);
								baby->Transform()->translate = go->Transform()->translate;
								baby->Transform()->scale = go->Transform()->scale - Vector3(1, 1, 1);
								kb = baby->GetComponent<CompKinematicBody>();
								kb->mass = go->GetComponent<CompKinematicBody>()->mass - 1;
								kb->force *= other->GetComponent<CompKinematicBody>()->MAX_LIN_VEL;
							}
						}
						go->ActiveSelf(false);
					}
				}
			
				//asteroid with missle	
				for (unsigned k = 0; k < POOL_SIZE[GO_MISSLE]; ++k)
				{
					GameObject* other = m_goList[GO_MISSLE][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score++;
						go->ActiveSelf(false);
						player->GetComponent<CompTracker>()->target = nullptr;
					}
				}
			}
			break;
		}
		case GO_ENEMY_KAMIKAZE: //with player, lmg, missle
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_ENEMY_KAMIKAZE][j];
				if (!go->IsActive())
					continue;
				//kamikaze enemy and player
				if (HelperPhysics::IsColliding(go, player))
				{
					go->GetComponent<CompHealth>()->hp = 0;				
					player->GetComponent<CompHealth>()->hp -= 10;
					player->GetComponent<CompKinematicBody>()->force = (player->Transform()->translate - go->Transform()->translate) * player->GetComponent<CompKinematicBody>()->FORCE_MAG;
				}

				//kamikaze and asteroid
				for (unsigned k = 0; k < POOL_SIZE[GO_ASTEROID]; ++k)
				{
					GameObject* other = m_goList[GO_ASTEROID][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						go->GetComponent<CompHealth>()->hp--;
						//papa asteroid
						if (other->Transform()->scale.x >= 3.f) //limit
						{
							GameObject* baby;
							CompKinematicBody* kb;
							for (int i = 0; i < Math::RandIntMinMax(2, 3); ++i)
							{
								baby = GOSpawn(GO_ASTEROID);
								baby->Transform()->translate = other->Transform()->translate;
								baby->Transform()->scale = other->Transform()->scale - Vector3(1, 1, 1);
								kb = baby->GetComponent<CompKinematicBody>();
								kb->mass = other->GetComponent<CompKinematicBody>()->mass - 1;
								kb->force *= go->GetComponent<CompKinematicBody>()->MAX_LIN_VEL;
							}
						}
						other->ActiveSelf(false);
					}
				}

				//kamikaze with lmg bullet
				for (unsigned k = 0; k < POOL_SIZE[GO_LMG_BULLET]; ++k)
				{
					GameObject* other = m_goList[GO_LMG_BULLET][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score += 2;
						go->GetComponent<CompHealth>()->hp--;
					}
				}

				//kamikaze with missle
				for (unsigned k = 0; k < POOL_SIZE[GO_MISSLE]; ++k)
				{
					GameObject* other = m_goList[GO_MISSLE][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score++;
						go->GetComponent<CompHealth>()->hp -= 10;
						player->GetComponent<CompTracker>()->target = nullptr;
					}
				}
			}
			break;
		}
		case GO_ENEMY_SHOOTING: //with player, lmg, missle
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_ENEMY_SHOOTING][j];
				if (!go->IsActive())
					continue;
				//enemy and player
				if (HelperPhysics::IsColliding(go, player))
				{
					go->GetComponent<CompHealth>()->hp = 0;
					player->GetComponent<CompHealth>()->hp -= 10;
					player->GetComponent<CompKinematicBody>()->force = (player->Transform()->translate - go->Transform()->translate) * player->GetComponent<CompKinematicBody>()->FORCE_MAG;
				}

				//enemy and asteroid
				for (unsigned k = 0; k < POOL_SIZE[GO_ASTEROID]; ++k)
				{
					GameObject* other = m_goList[GO_ASTEROID][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						go->GetComponent<CompHealth>()->hp--;
						//papa asteroid
						if (other->Transform()->scale.x >= 3.f) //limit
						{
							GameObject* baby;
							CompKinematicBody* kb;
							for (int i = 0; i < Math::RandIntMinMax(2, 3); ++i)
							{
								baby = GOSpawn(GO_ASTEROID);
								baby->Transform()->translate = other->Transform()->translate;
								baby->Transform()->scale = other->Transform()->scale - Vector3(1, 1, 1);
								kb = baby->GetComponent<CompKinematicBody>();
								kb->mass = other->GetComponent<CompKinematicBody>()->mass - 1;
								kb->force *= go->GetComponent<CompKinematicBody>()->MAX_LIN_VEL;
							}
						}
						other->ActiveSelf(false);
					}
				}

				//enemy with lmg bullet
				for (unsigned k = 0; k < POOL_SIZE[GO_LMG_BULLET]; ++k)
				{
					GameObject* other = m_goList[GO_LMG_BULLET][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score += 2;
						go->GetComponent<CompHealth>()->hp--;
					}
				}

				//enemy with missle
				for (unsigned k = 0; k < POOL_SIZE[GO_MISSLE]; ++k)
				{
					GameObject* other = m_goList[GO_MISSLE][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						m_score++;
						go->GetComponent<CompHealth>()->hp -= 10;
						player->GetComponent<CompTracker>()->target = nullptr;
					}
				}
			}
			break;
		}
		case GO_LMG_ENEMY: //with player
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_LMG_ENEMY][j];
				if (!go->IsActive())
					continue;

				//bullet with player
				if (HelperPhysics::IsColliding(go, player))
				{
					player->GetComponent<CompHealth>()->hp -= 1;
					go->ActiveSelf(false);										
				}

				//bullet enemy with player lmg bullet		
				for (unsigned k = 0; k < POOL_SIZE[GO_LMG_BULLET]; ++k)
				{
					GameObject* other = m_goList[GO_LMG_BULLET][k];
					if (!other->IsActive())
						continue;

					if (HelperPhysics::IsColliding(go, other))
					{
						other->ActiveSelf(false);
						go->ActiveSelf(false);
					}
				}
			}
			break;
		}
		case GO_BLACKHOLE: //with everything except bg
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_BLACKHOLE][j];
				if (!go->IsActive())
					continue;


				//blackhole and player								
				CompTransform* t1 = go->Transform();
				CompTransform* t2 = player->Transform();
				if (HelperPhysics::IsColliding(go, player, -t1->scale.x * t1->scale.y - t2->scale.x * t2->scale.y + 25))
				{
					m_lives = 0;
				}

				//blackhole and all other objects
				GameObject* other;
				for (int i = GO_SHIP + 1; i < GO_TOTAL; ++i)
				{
					if (i == GO_BLACKHOLE)
						continue;
					for (unsigned k = 0; k < POOL_SIZE[i]; ++k)
					{
						other = m_goList[(GAMEOBJECT_ID)i][k];

						if (!other->IsActive())
							continue;

						CompTransform* t1 = go->Transform();
						CompTransform* t2 = other->Transform();

						if (HelperPhysics::IsColliding(go, other, -t1->scale.x * t1->scale.y - t2->scale.x * t2->scale.y + 64))
						{
							other->ActiveSelf(false);
						}
					}
				}
			}
			break;
		}
		case GO_BUDDY_KAMIKAZE: //with everything except bg and player
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_BUDDY_KAMIKAZE][j];
				if (!go->IsActive())
					continue;

				GameObject* other;
				for (int i = GO_SHIP + 1; i < GO_TOTAL; ++i)
				{
					if (i == GO_BUDDY_KAMIKAZE)
						continue;
					for (unsigned k = 0; k < POOL_SIZE[i]; ++k)
					{
						other = m_goList[(GAMEOBJECT_ID)i][k];

						if (!other->IsActive())
							continue;

						if (HelperPhysics::IsColliding(go, other))
						{
							go->ActiveSelf(false);
							other->ActiveSelf(false);
							GameObject* bh = GOSpawn(GO_BLACKHOLE);
							bh->Transform()->translate = go->Transform()->translate;
							k = POOL_SIZE[i]; //break;
						}
					}
				}
			}
			break;
		}
		case GO_POWERUP:
		{
			for (unsigned j = 0; j < size; ++j)
			{
				GameObject* go = m_goList[GO_POWERUP][j];
				if (!go->IsActive())
					continue;

				CompPowerup* pwup = go->GetComponent<CompPowerup>();

				//powerup with player
				if (HelperPhysics::IsColliding(go, player))
				{
					go->ActiveSelf(false);
					switch (pwup->type)
					{
					case pwup->PWUP_MISSLE_REFILL:
						player->GetComponent<CompAmmo>()->ammo[AMMO_MISSLE] = player->GetComponent<CompAmmo>()->MAX_AMMO[AMMO_MISSLE];					
						break;
					case pwup->PWUP_BUDDY_HEAL:
						if (!m_goList[GO_BUDDY_HEAL][0]->IsActive())
							GOSpawn(GO_BUDDY_HEAL);
						break;
					case pwup->PWUP_BUDDY_KAMIKAZE:
						if (!m_goList[GO_BUDDY_KAMIKAZE][0]->IsActive())
							GOSpawn(GO_BUDDY_KAMIKAZE);
						break;
					case pwup->PWUP_BARRIER:
						if (!m_goList[GO_REPULSOR][0]->IsActive())
							GOSpawn(GO_REPULSOR);
						break;
					}
					break;
				}
			}
		}
	}
}

void ScenePhysics::PlayerController(double dt)
{
	CompKinematicBody* kb = player->GetComponent<CompKinematicBody>();
	//controls
	if (Application::IsKeyPressed('W'))
	{
		kb->force += player->Transform()->dir * kb->FORCE_MAG;
	}
	if (Application::IsKeyPressed('S'))
	{
		kb->force += -player->Transform()->dir * kb->FORCE_MAG * 0.5f;
	}
	if (Application::IsKeyPressed('A'))
	{
		kb->force += player->Transform()->dir * kb->TORQUE_MAG;
		kb->torque += (Vector3(1, -1, 0).Cross(Vector3(0, kb->TORQUE_MAG, 0))).z;
	}
	if (Application::IsKeyPressed('D'))
	{
		kb->force += player->Transform()->dir * kb->TORQUE_MAG;
		kb->torque += (Vector3(-1, -1, 0).Cross(Vector3(0, kb->TORQUE_MAG, 0))).z;
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		CompTimer* timer = player->GetComponent<CompTimer>();
		if (timer->deltaTime[0] > timer->targetTime[0])
		{
			GOSpawn(GO_LMG_BULLET);
			timer->deltaTime[0] = 0.0;
		}
	}
	else if (Application::IsKeyPressed('E') && player->GetComponent<CompAmmo>()->ammo[AMMO_MISSLE] > 0)
	{
		CompTimer* timer = player->GetComponent<CompTimer>();
		if (timer->deltaTime[1] > timer->targetTime[1])
		{			
			GOSpawn(GO_MISSLE);		
			timer->deltaTime[1] = 0.0;
		}
	}

	//controllable buddy ship
	GameObject* buddy = m_goList[GO_BUDDY_KAMIKAZE][0];
	if (buddy->IsActive())
	{
		kb = buddy->GetComponent<CompKinematicBody>();
		if (Application::IsKeyPressed(VK_UP))
		{
			kb->force += buddy->Transform()->dir * kb->FORCE_MAG;
		}
		if (Application::IsKeyPressed(VK_DOWN))
		{
			kb->force += -buddy->Transform()->dir * kb->FORCE_MAG * 0.5f;
		}
		if (Application::IsKeyPressed(VK_LEFT))
		{
			kb->force += buddy->Transform()->dir * kb->TORQUE_MAG;
			kb->torque += (Vector3(1, -1, 0).Cross(Vector3(0, kb->TORQUE_MAG, 0))).z;
		}
		if (Application::IsKeyPressed(VK_RIGHT))
		{
			kb->force += buddy->Transform()->dir * kb->TORQUE_MAG;
			kb->torque += (Vector3(-1, -1, 0).Cross(Vector3(0, kb->TORQUE_MAG, 0))).z;
		}
	}
}

void ScenePhysics::PlayerReset()
{
	player->Transform()->translate.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
	player->Transform()->scale.Set(4, 6, 1);
	player->Transform()->dir.Set(0, 1, 0);

	CompKinematicBody* kb = player->GetComponent<CompKinematicBody>();
	kb->torque = kb->angVel = kb->prevAngVel = 0;
	kb->force.SetZero();
	kb->linearVel.SetZero();
	kb->prevLinearVel.SetZero();

	kb->mass = 1;
	kb->momentOfInertia = 0.5f * kb->mass * player->Transform()->scale.x * player->Transform()->scale.y;
	kb->FORCE_MAG = 100;
	kb->TORQUE_MAG = 50;
	kb->MAX_LIN_VEL = 25;
	kb->MAX_ANG_VEL = 2;

	player->GetComponent<CompHealth>()->hp = 100;
	player->GetComponent<CompHealth>()->MAX_HP = 100;

	CompTimer* timer;
	timer = player->GetComponent<CompTimer>();
	timer->targetTime[0] = 0.1;
	timer->targetTime[1] = 0.5;

	CompAmmo* amm;
	amm = player->GetComponent<CompAmmo>();
	amm->MAX_AMMO[AMMO_MISSLE] = amm->ammo[AMMO_MISSLE] = 3;
}
