#ifndef CAMERA_H
#define CAMERA_H

#include "../Node.h"
#include "../../Utility/TypeID.h"
#include "../../Utility/Component.h"
#include "../../Utility/Math/Mtx44.h"

class Axis;
class Transform;
class Camera : public Node, public TypeID<Camera>, public Component
{
public:
	Camera(std::string name = "Camera");
	~Camera();
	
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();

	enum MODE
	{
		DEBUG = 0,		
		CUSTOM,
		NUM_MODE		
	};
	Camera* SetMode(MODE m);
	Camera* SetSpeed(float s);
	Mtx44* GetViewMtx();
private:
	MODE mode;
	float speed;

	Transform* t;
	Axis* axis;
	Mtx44 view;
};

#endif