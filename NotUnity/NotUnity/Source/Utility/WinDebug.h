#ifndef WIN_DEBUG_H
#define WIN_DEBUG_H

#include <windows.h>
#include <iostream>
#include <string>

//this handles the use of std::cout to print debugging information
//first variable is whatever that has operator<< overloaded to return an std::ostream
//second variable is whether to show the console over everything
//errors will throw an exception, halting the program
/********************************************
  EXAMPLE USAGE

  Debug::Log("Hello World!", true);
  Debug::LogWarning("Hello World!", false);
  Debug::LogError("Hello World!");
********************************************/


//macros to get file and line num, with overloading

#define ARG_STRIPPER(arg1, arg2, arg3, ...) arg3

#define LOG_1_ARGS(msg) _Log(__LINE__, __FILE__, msg)
#define LOG_2_ARGS(msg, popup) _Log(__LINE__, __FILE__, msg, popup)
#define LOG_MACRO_CHOOSER(...) ARG_STRIPPER(__VA_ARGS__, LOG_2_ARGS, LOG_1_ARGS)
#define Log(...) LOG_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define LOGW_1_ARGS(msg) _LogWarning(__LINE__, __FILE__, msg)
#define LOGW_2_ARGS(msg, popup) _LogWarning(__LINE__, __FILE__, msg, popup)
#define LOGW_MACRO_CHOOSER(...) ARG_STRIPPER(__VA_ARGS__, LOGW_2_ARGS, LOGW_1_ARGS)
#define LogWarning(...) LOGW_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define LOGE_1_ARGS(msg) _LogError(__LINE__, __FILE__, msg)
#define LOGE_2_ARGS(msg, popup) _LogError(__LINE__, __FILE__, msg, popup)
#define LOGE_MACRO_CHOOSER(...) ARG_STRIPPER(__VA_ARGS__, LOGE_2_ARGS, LOGE_1_ARGS)
#define LogError(...) LOGE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#if _DEBUG
class Debug
{
public:
	Debug() {}
	~Debug() {}

	template <typename T>
	inline static void _Log(int line, const char* fileName, T input, bool popup = false)
	{
		if (popup) SetConsoleTop();		
		OutputFileInfo(line, fileName);
		SetColor(1, 1, 1, 1);				
		std::cout << input << std::endl;
	}
	template <typename T>
	inline static void _LogWarning(int line, const char* fileName, T input, bool popup = true)
	{
		if (popup) SetConsoleTop();
		OutputFileInfo(line, fileName);
		SetColor(1, 1, 0, 1);
		std::cout << input << std::endl;
	}
	template <typename T>
	inline static void _LogError(int line, const char* fileName, T input, bool popup = true)
	{
		if (popup) SetConsoleTop();
		OutputFileInfo(line, fileName);
		SetColor(1, 0, 0, 1);		
		std::cout << input << std::endl;
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
	inline static void OutputFileInfo(int line, const char* fileName)
	{
		//find file name without path
		std::string temp = fileName;
		int idx = temp.length();
		for (auto it = temp.rbegin(); it != temp.rend(); ++it)
		{
			*it = ::toupper(*it);
			if (*it != '/' && *it != '\\') --idx;
			else break;
		}
		//set le colors
		SetColor(1, 1, 1, 0);
		std::cout << " [ ";
		SetColor(0, 1, 0, 1);
		std::cout << temp.substr(idx, temp.length() - 1);
		SetColor(1, 1, 1, 0);
		std::cout << " | ";
		SetColor(0, 1, 1, 1);
		std::cout << "Line " + std::to_string(line);
		SetColor(1, 1, 1, 0);
		std::cout << " ] ";
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
	inline static void _Log(T input) {}
	template <typename T>
	inline static void _LogWarning(T input) {}
	template <typename T>
	inline static void _LogError(T input) {}
};
#endif


#endif