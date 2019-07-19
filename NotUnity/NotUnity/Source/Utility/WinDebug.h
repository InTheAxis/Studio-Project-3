#ifndef WIN_DEBUG_H
#define WIN_DEBUG_H

#include "windows.h"
#include <iostream>
#include <string>

//this handles the use of std::cout to print debugging information
//first variable is whatever that has operator<< overloaded to return an std::ostream
//second variable is whether to show the console over everything
//errors will throw an exception, halting the program
/********************************************
  EXAMPLE USAGE

  Debug::Log("Hello World!", true);
********************************************/

#if _DEBUG
class Debug
{
public:
	Debug() {}
	~Debug() {}

	//TODO provide line number and file name

	template <typename T>
	inline static void Log(T input, bool popup = false)
	{
		if (popup) SetConsoleTop();
		SetColor(1, 1, 1, 1);
		std::cout << " " << input << std::endl;
	}
	template <typename T>
	inline static void LogWarning(T input, bool popup = true)
	{
		if (popup) SetConsoleTop();
		SetColor(1, 1, 0, 1);
		std::cout << " " << input << std::endl;
	}
	template <typename T>
	inline static void LogError(T input, bool popup = true)
	{
		if (popup) SetConsoleTop();
		SetColor(1, 0, 0, 1);
		std::cout << " " << input << std::endl;
		DebugBreak();
	}
private:
	inline static void SetColor(bool red = 1, bool green = 1, bool blue = 1, bool intense = 1)
	{
		WORD colorFlags = 0x0000;
		if (red) colorFlags |= FOREGROUND_RED;
		if (blue) colorFlags |= FOREGROUND_BLUE;
		if (green) colorFlags |= FOREGROUND_GREEN;
		if (intense) colorFlags |= FOREGROUND_INTENSITY;
		//thanks https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
		::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorFlags);
	}
	inline static void SetConsoleTop()
	{
		//thanks https://stackoverflow.com/questions/27068650/c-console-application-always-on-top/27068814#27068814
		::SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		::ShowWindow(GetConsoleWindow(), SW_NORMAL);
	}
};
#else
//dummy class
class Debug
{
public:
	Debug() {}
	~Debug() {}	
	template <typename T>
	inline static void Log(T input) {}
	template <typename T>
	inline static void LogWarning(T input) {}
	template <typename T>
	inline static void LogError(T input) {}
	inline static void SetColor(bool red = 1, bool green = 1, bool blue = 1, bool intense = 1);
	inline static void SetConsoleTop();
};
#endif


#endif