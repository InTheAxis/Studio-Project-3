#ifndef DELEGATE_H
#define DELEGATE_H

#include <vector>

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

#endif