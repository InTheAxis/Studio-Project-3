#ifndef SCRIPT_BASE_H
#define SCRIPT_BASE_H

#include "CompBase.h"

//script extends the component class
//scriptbase is an abstract class defining the extensions
//all scripts should inherit from Script<T> to get unique id
/********************************************
  EXAMPLE USAGE

class ScriptExample : public Script<ScriptExample>
{
public:
	ScriptExample() {}
	~ScriptExample() {}

	//optional virtuals
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}

	float myVar;
};
********************************************/

class ScriptBase
{
public:
	//optional virtuals
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
};

template <typename T>
class Script : public Component<T>, public ScriptBase
{
};

//test scripts below
class ScriptTest : public Script<ScriptTest>
{
public:
	ScriptTest() {}
	~ScriptTest() {}

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
};
class ScriptTest2 : public Script<ScriptTest2>
{
public:
	ScriptTest2() {}
	~ScriptTest2() {}

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
};

#endif