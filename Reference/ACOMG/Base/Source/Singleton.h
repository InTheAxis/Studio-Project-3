#ifndef SINGLETON_H
#define SINGLETON_H

//thanks to https://stackoverflow.com/questions/41328038/singleton-template-as-base-class-in-c

template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T instance;
		return instance;
	}

	virtual void Init() = 0;
	virtual void Drop() = 0;

	Singleton(Singleton const &) = delete;
	Singleton& operator=(Singleton const &) = delete;
protected:
	Singleton() {}
	~Singleton() {}
};
#endif