#include "ControllerKeyboard.h"

ControllerKeyboard::ControllerKeyboard()
{
}

ControllerKeyboard::~ControllerKeyboard()
{
}

void ControllerKeyboard::UpdateKeyboardStatus(unsigned char _slot, bool _isPressed)
{
	currStatus.set(_slot, _isPressed);
}

void ControllerKeyboard::EndFrameUpdate()
{
	prevStatus = currStatus;
}

bool ControllerKeyboard::IsKeyDown(unsigned char _slot)
{
	return currStatus.test(_slot);
}

bool ControllerKeyboard::IsKeyUp(unsigned char _slot)
{
	return !currStatus.test(_slot);
}

bool ControllerKeyboard::IsKeyPressed(unsigned char _slot)
{
	return IsKeyDown(_slot) && !prevStatus.test(_slot);
}

bool ControllerKeyboard::IsKeyReleased(unsigned char _slot)
{
	return IsKeyUp(_slot) && prevStatus.test(_slot);
}