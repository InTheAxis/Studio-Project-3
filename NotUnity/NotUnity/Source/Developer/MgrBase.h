#ifndef MGR_BASE_H
#define MGR_BASE_H

#include "WinDebug.h"
#include "Singleton.h"

//base for all managers
//make sure to inherit from Mgr<T> to get correct singleton instance
/********************************************
  EXAMPLE USAGE

class MgrExample : public Mgr<MgrExample>
{
	friend class Singleton<MgrExample>;
public:
	virtual std::string Log(std::string name = "");
	float myVar;
private:
	MgrExample() {}
	~MgrExample() {}
};
********************************************/

class MgrBase 
{
public:
	virtual std::string Log(std::string name = "") = 0;
};

template <typename T>
class Mgr : public Singleton<T>, public MgrBase
{	
};

#endif