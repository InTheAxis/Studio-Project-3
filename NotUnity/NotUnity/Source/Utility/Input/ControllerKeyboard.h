#ifndef CONTROLLER_KEYBOARD_H
#define CONTROLLER_KEYBOARD_H

#include "../Singleton.h"
#include <bitset>
#include <windows.h>

//controls the status of the keyboard, use this singleton to get keyboard input (under User Interface)

class ControllerKeyboard : public Singleton<ControllerKeyboard>
{
	friend Singleton<ControllerKeyboard>;
public:
	const static int MAX_KEYS = 256;

	// System Interface
	void UpdateKeyboardStatus(unsigned char _slot, bool _isPressed);
	void EndFrameUpdate();
	
	// User Interface
	bool IsKeyDown(unsigned char _slot);
	bool IsKeyUp(unsigned char _slot);
	bool IsKeyPressed(unsigned char _slot);
	bool IsKeyReleased(unsigned char _slot);

private:
	ControllerKeyboard();
	~ControllerKeyboard();

	std::bitset<MAX_KEYS> currStatus, prevStatus;
};

#endif