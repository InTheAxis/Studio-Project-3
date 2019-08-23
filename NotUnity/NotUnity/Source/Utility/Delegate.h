#ifndef DELEGATE_H
#define DELEGATE_H

#include <map>
#include <string>
#include <functional>

/*
	Delegates:
	Use this class to create callback functions
	Declare a Delegate object, then you can
	subcribe functions to it by passing in its function pointer
	and object (if it's a member function).
	Label your callback function with a event name, for easy removal later
	Remember to unsubscribe the function with its event name
*/

//for one of any arg type
template <typename T>
class Delegate
{
	typedef std::function<void(T)> fptr;
public:
	Delegate() {}
	~Delegate() { callbacks.clear(); }
	void operator()(T arg)
	{
		for (auto& f : callbacks)
		{
			if (!toDelete[f.first])
				f.second(arg);
		}
		for (auto& d : toDelete)
		{
			if (d.second)
				callbacks.erase(d.first);
		}
	}

	void Subscribe(void(*func)(T), std::string event)
	{
		if (callbacks.count(event) > 0)
			callbacks.erase(event);
		callbacks[event] = func;
		toDelete[event] = false;
	}

	template <typename t>
	void Subscribe(void(t::*func)(T), t* object, std::string event)
	{
		if (callbacks.count(event) > 0)
			callbacks.erase(event);
		callbacks[event] = (std::bind(func, object, std::placeholders::_1));
		toDelete[event] = false;
	}

	void UnSubscribe(std::string event)
	{		
		if (callbacks.count(event) > 0)
			toDelete[event] = true;
	}

private:
	std::map<std::string, fptr> callbacks;
	std::map<std::string, bool> toDelete;
};

//specialise for void args
template <>
class Delegate<void>
{
	typedef std::function<void()> fptr;
public:
	Delegate() {}
	~Delegate() { callbacks.clear(); }
	void operator()()
	{
		for (auto& f : callbacks)
		{
			if (!toDelete[f.first])
				f.second();
		}
		for (auto& d : toDelete)
		{
			if (d.second)
				callbacks.erase(d.first);
		}
	}

	void Subscribe(void(*func)(), std::string event)
	{
		if (callbacks.count(event) > 0)
			callbacks.erase(event);
		callbacks[event] = func;
		toDelete[event] = false;
	}

	template <typename t>
	void Subscribe(void(t::*func)(), t* object, std::string event)
	{
		if (callbacks.count(event) > 0)
			callbacks.erase(event);
		callbacks[event] = (std::bind(func, object, std::placeholders::_1));
		toDelete[event] = false;
	}

	void UnSubscribe(std::string event)
	{
		if (callbacks.count(event) > 0)
			toDelete[event] = true;
	}

private:
	std::map<std::string, fptr> callbacks;
	std::map<std::string, bool> toDelete;
};
#endif