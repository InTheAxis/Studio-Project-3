#ifndef ColiderRender_H
#define ColliderRender_H

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