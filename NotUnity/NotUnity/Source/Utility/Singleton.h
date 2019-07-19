#ifndef SINGLETON_H
#define SINGLETON_H

//thanks to https://stackoverflow.com/questions/41328038/singleton-template-as-base-class-in-c
/********************************************
  EXAMPLE USAGE

class EXAMPLE : public Singleton<EXAMPLE>
{
	friend class Singleton<EXAMPLE>;
public:
	//stuff
private:
	EXAMPLE() {}
	~EXAMPLE() {}
};

   ... in other files,
   EXAMPLE::Instance().Init();
********************************************/

template<typename T>
class Singleton
{
public:
	static T* Instance()
	{
		static T instance;
		return &instance;
	}

	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;
protected:
	Singleton() {}
	~Singleton() {}
};
#endif