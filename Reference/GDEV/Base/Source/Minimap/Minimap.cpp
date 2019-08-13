#include "Minimap.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"

// Constructor
CMinimap::CMinimap(void)
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
	, m_cMinimap_Avatar(NULL)
	, m_cMinimap_Target(NULL)
	, m_cMinimap_Stencil(NULL)
	, m_iAngle(0)
	, m_iZoom(1)
	, mode(MODE_2D)	
	, playerInfo(NULL)
{	
}

// Destructor
CMinimap::~CMinimap(void)
{
	// Set this to NULL only. Let SceneText delete the instance	

	if (m_cMinimap_Background)
	{
		delete m_cMinimap_Background;
		m_cMinimap_Background = NULL;
	}
	if (m_cMinimap_Border)
	{
		delete m_cMinimap_Border;
		m_cMinimap_Border = NULL;
	}
	if (m_cMinimap_Avatar)
	{
		delete m_cMinimap_Avatar;
		m_cMinimap_Avatar = NULL;
	}
	if (m_cMinimap_Target)
	{
		delete m_cMinimap_Target;
		m_cMinimap_Target = NULL;
	}
}

// Initialise this class instance
bool CMinimap::Init(CPlayerInfo* playerInfo)
{
	m_iAngle = 0;
	m_iZoom = 1;
	position.Set(335.f, 235.f, 0.0f);
	scale.Set(100.0f, 100.0f, 100.0f);
	this->playerInfo = playerInfo;
	return true;
}

bool CMinimap::SetTarget(Mesh* aTarget)
{
	if(aTarget != NULL)
	{
		m_cMinimap_Target = aTarget;
		return true;
	}
	return false;
}
Mesh* CMinimap::GetTarget(void) const
{
	return m_cMinimap_Target;
}
// Set the background mesh to this class
bool CMinimap::SetBackground(Mesh* aBackground)
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh* CMinimap::GetBackground(void) const
{
	return m_cMinimap_Background;
}
// Set the Border mesh to this class
bool CMinimap::SetBorder(Mesh* aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the Border mesh to this class
Mesh* CMinimap::GetBorder(void) const
{
	return m_cMinimap_Border;
}
// Set the Avatar mesh to this class
bool CMinimap::SetAvatar(Mesh* anAvatar)
{
	if (anAvatar != NULL)
	{
		m_cMinimap_Avatar = anAvatar;
		return true;
	}
	return false;
}
// Get the Avatar mesh to this class
Mesh* CMinimap::GetAvatar(void) const
{
	return m_cMinimap_Avatar;
}

// Set m_iAngle of avatar
bool CMinimap::SetAngle(const int m_iAngle)
{
	this->m_iAngle = m_iAngle;
	return true;
}
// Get m_iAngle
int CMinimap::GetAngle(void) const
{
	return m_iAngle;
}

// Set Zoom of Minimap
bool CMinimap::SetZoom(const int m_iZoom)
{
	this->m_iZoom = m_iZoom;
	return true;
}
// Get m_fZoom
int CMinimap::GetZoom(void) const
{
	return m_iZoom;
}

// Set the Stencil mesh to this class
bool CMinimap::SetStencil(Mesh* aStencil)
{
	if (aStencil != NULL)
	{
		m_cMinimap_Stencil = aStencil;
		return true;
	}
	return false;
}
// Get the Stencil mesh to this class
Mesh* CMinimap::GetStencil(void) const
{
	return m_cMinimap_Stencil;
}

// Set the pointer to the enemies to this class instance
void CMinimap::SetEnemies(CEnemy3D* anEnemy[], int num)
{
	numEnemy = num;	
	this->anEnemy = &(anEnemy[0]);
}


// Set position of minimap
void CMinimap::SetPosition(const int x, const int y)
{
	position.Set((float)x, (float)y, 0.0f);
}

// Get position of minimap
Vector3 CMinimap::GetPosition(void) const
{
	return position;
}

// Set size of minimap
void CMinimap::SetSize(const int size_x, const int size_y)
{
	scale.Set((float)size_x, (float)size_y, 0.0f);
}

// Get size of minimap
Vector3 CMinimap::GetSize(void) const
{
	return scale;
}

// Render the minimap as an UI
void CMinimap::RenderUI()
{
	if (mode == MODE_3D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Push the current transformation into the modelStack
	modelStack.PushMatrix();
		// Translate the current transformation
		modelStack.Translate(position.x, position.y, position.z);
		// Scale the current transformation
		modelStack.Scale(scale.x, scale.y, scale.z);

		// Push the current transformation into the modelStack
		modelStack.PushMatrix();

			// Activate stencil rendering
			ActivateStencilRendering();
			// Initialise the stencil rendering
			PreStencilRendering();

			if (m_cMinimap_Stencil)
				RenderHelper::RenderMesh(m_cMinimap_Stencil);

			// DeInitialise the stencil rendering
			PostStencilRendering();

			modelStack.PushMatrix();
				// Rotate the current transformation
				modelStack.Rotate(m_iAngle+90, 0.0f, 0.0f, 1.0f);
				if (m_cMinimap_Background)
					RenderHelper::RenderMesh(m_cMinimap_Background);
			modelStack.PopMatrix();

			// Disable depth test
			glDisable(GL_DEPTH_TEST);

			// Render enemies
			modelStack.PushMatrix();
				// Rotate the current transformation
				modelStack.Rotate(m_iAngle+90, 0.0f, 0.0f, 1.0f);
				// Render an enemy
				RenderEnemies();
			modelStack.PopMatrix();

			// Display the Avatar
			if (m_cMinimap_Avatar)
				RenderHelper::RenderMesh(m_cMinimap_Avatar);

			// Enable depth test
			glEnable(GL_DEPTH_TEST);

			// DeActivate stencil rendering
			DeActivateStencilRendering();

		modelStack.PopMatrix();

		// Render a border
		if (m_cMinimap_Border)
			RenderHelper::RenderMesh(m_cMinimap_Border);

	modelStack.PopMatrix();
}

// Activate stencil rendering
void CMinimap::ActivateStencilRendering(void)
{
	// Enable stencil mode
	glEnable(GL_STENCIL_TEST);
	// Switch on alpha test
	glEnable(GL_ALPHA_TEST);
}

// Pre stencil rendering
void CMinimap::PreStencilRendering(void)
{
	// Configure stencil mode
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glAlphaFunc(GL_GREATER, 0x0A);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Don't write to colour buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
}

// Post stencil rendering
void CMinimap::PostStencilRendering(void)
{
	// Switch off stencil function
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glAlphaFunc(GL_LEQUAL, 0x0A);
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Write to colour buffer
	glDepthMask(GL_TRUE); // Write to depth buffer
}

// DeActivate stencil rendering
void CMinimap::DeActivateStencilRendering(void)
{
	// Switch off alpha test
	glDisable(GL_ALPHA_TEST);
	// Disable stencil test
	glDisable(GL_STENCIL_TEST);
}

// Render the enemies
void CMinimap::RenderEnemies(void)
{
	if (numEnemy <= 0)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	for (int i = 0; i < numEnemy; ++i)
	{
		if (anEnemy[i]->IsDone())
			continue;
		modelStack.PushMatrix();
		// Render an enemy
		Vector3 DistTranslate = anEnemy[i]->GetPosition() - playerInfo->GetPos();
		DistTranslate *= 0.01f + m_iZoom * 0.01f;
		modelStack.Translate(DistTranslate.x, -DistTranslate.z, 0.0f);
		//modelStack.Scale(0.25f, 0.25f, 0.25f);
		RenderHelper::RenderMesh(m_cMinimap_Target);
		modelStack.PopMatrix();
	}
}

// Create a minimap
CMinimap* Create::Minimap(const bool m_bAddToLibrary)
{
	CMinimap* result = CMinimap::GetInstance();
	if (m_bAddToLibrary)
		EntityManager::GetInstance()->AddEntity(result);
	return result;
}
