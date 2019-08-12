#ifndef MGR_MAIN_H
#define MGR_MAIN_H

#include "Manager.h"

class MgrMain : public Manager<MgrMain>
{
	friend Singleton<MgrMain>;
public:
	virtual void Start();
	virtual void Update(double dt);
	virtual void End();	
protected:
	MgrMain(std::string name = "MgrMain");
	~MgrMain();

};
#endif