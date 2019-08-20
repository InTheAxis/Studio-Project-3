#ifndef COL_RENDER_H
#define COL_RENDER_H

#include "Renderable.h"

class MgrGraphics;
class ColliderRender : public Renderable
{
public:
	ColliderRender(std::string name = "ColliderRender");
	~ColliderRender();

	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	virtual void Render();

	ColliderRender* AttachPoints(std::vector<Vector3> pts);
};

#endif