#ifndef DELEGATE_H
#define DELEGATE_H

#include <vector>

//for one of any arg type

template <typename T>
class Delegate
{
public:
	Delegate() {}
	~Delegate() { callbacks.clear(); }

	void operator()(T arg)
	{
		for (auto f : callbacks)
			f(arg);
	}
	void operator+=(void(*func)(T))
	{
		callbacks.emplace_back(func);
	}
	void operator-=(void(*func)(T))
	{
		for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			if (*it == func)
			{
				callbacks.erase(it);
				return;
			}
		}
	}
private:
	std::vector<void(*)(T)> callbacks;
};

//specialise for void args
template <>
class Delegate<void>
{
public:
	Delegate() {}
	~Delegate() { callbacks.clear(); }

	void operator()()
	{
		for (auto f : callbacks)
			f();
	}
	void operator+=(void(*func)())
	{
		callbacks.emplace_back(func);
	}
	void operator-=(void(*func)())
	{
		for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		{
			if (*it == func)
			{
				callbacks.erase(it);
				return;
			}
		}
	}
private:
	std::vector<void(*)()> callbacks;
};

#endif