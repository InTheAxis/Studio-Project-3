#pragma once

#include "EntityBase.h"
#include "SingletonTemplate.h"
#include "Mesh.h"
#include "..\Enemy\Enemy3D.h"

class CPlayerInfo;
class CMinimap : public EntityBase, public Singleton<CMinimap>
{
public:
	enum SPRITE_RENDERMODE
	{
		MODE_2D,
		MODE_3D,
		NUM_MODE
	};

	CMinimap(void);
	virtual ~CMinimap(void);

	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;
	Mesh* m_cMinimap_Avatar;
	Mesh* m_cMinimap_Target;
	Mesh* m_cMinimap_Stencil;

	// Initialise this class instance
	bool Init(CPlayerInfo* playerInfo);

	// Set the target (a.k.a. the enemies in the minimap)
	bool SetTarget(Mesh* aTarget);
	// Get the target (a.k.a. the enemies in the minimap)
	Mesh* GetTarget(void) const;

	// Set the background mesh to this class
	bool SetBackground(Mesh* aBackground);
	// Get the background mesh to this class
	Mesh* GetBackground(void) const;

	// Set the Border mesh to this class
	bool SetBorder(Mesh* aBorder);
	// Get the Border mesh to this class
	Mesh* GetBorder(void) const;

	// Set the Avatar mesh to this class
	bool SetAvatar(Mesh* anAvatar);
	// Get the Avatar mesh to this class
	Mesh* GetAvatar(void) const;

	// Set rotation angle of minimap
	bool SetAngle(const int angle);
	// Get rotation angle of minimap
	int GetAngle(void) const;

	// Set Zoom of Minimap
	bool SetZoom(const int m_iZoom);
	// Get m_iZoom
	int GetZoom(void) const;

	// Set the Stencil mesh to this class
	bool SetStencil(Mesh* aStencil);
	// Get the Stencil mesh to this class
	Mesh* GetStencil(void) const;

	// Set the pointer to the enemies to this class instance
	void SetEnemies(CEnemy3D* anEnemy[], int num);

	// Set position of minimap
	void SetPosition(const int x, const int y);
	// Get position of minimap
	Vector3 GetPosition(void) const;

	// Set size of minimap
	void SetSize(const int size_x, const int size_y);
	// Get size of minimap
	Vector3 GetSize(void) const;

	// Render the minimap as an UI
	virtual void RenderUI();

protected:
	// Rotation from First Angle
	int m_iAngle;
	// Zoom value of the Minimap
	int m_iZoom;

	int numEnemy;

	SPRITE_RENDERMODE mode;

	CPlayerInfo* playerInfo;

	// Activate stencil rendering
	void ActivateStencilRendering(void);
	// Pre stencil rendering
	void PreStencilRendering(void);
	// Post stencil rendering
	void PostStencilRendering(void);
	// DeActivate stencil rendering
	void DeActivateStencilRendering(void);

	// Pointer to the enemies
	CEnemy3D** anEnemy;
	// Render the enemies
	void RenderEnemies(void);
};

namespace Create
{
	CMinimap* Minimap(const bool m_bAddToLibrary = true);
};
