#include "Keyboard.h"
#include <iostream>
using namespace std;

#include "KeyboardController.h"
#include "../../Base/Source/PlayerInfo/PlayerInfo.h"

CKeyboard::CKeyboard()
{
}


CKeyboard::~CKeyboard()
{
}

// Create this controller
bool CKeyboard::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	cout << "CKeyboard::Create()" << endl;
	return false;
}


// Read from the controller
int CKeyboard::Read(const float deltaTime)
{
	CController::Read(deltaTime);	
	
	// Process the keys for customisation
	if (KeyboardController::GetInstance()->IsKeyDown('W'))
	{
		Move_FrontBack(1);
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LSHIFT))
			Sprint();
	}
	if (KeyboardController::GetInstance()->IsKeyDown('S'))
		Move_FrontBack(-1);
	if (KeyboardController::GetInstance()->IsKeyDown('A'))
		Move_LeftRight(-1);
	if (KeyboardController::GetInstance()->IsKeyDown('D'))
		Move_LeftRight(1);

	if (KeyboardController::GetInstance()->IsKeyDown(VK_LCONTROL))
		Crouch();
	else if (KeyboardController::GetInstance()->IsKeyDown('Z'))
		Prone();
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
		Jump();
	else
		Idle();

	if (KeyboardController::GetInstance()->IsKeyReleased('R'))
		Reload();
	if (KeyboardController::GetInstance()->IsKeyReleased('Q'))
		AmmoPickUp();

	int tempX = 0, tempY = 0;
	
	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
		tempY = -1;
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
		tempY = 1;		
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
		tempX = -1;			
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
		tempX = 1;			

	UpdateArrowKeyDelta(tempX * 50, tempY * 50);
	
	return 0;
}
