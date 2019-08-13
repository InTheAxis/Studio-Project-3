#include "Mouse.h"
#include <iostream>
using namespace std;

#include "MouseController.h"
#include "../../Base/Source/PlayerInfo/PlayerInfo.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

// Create this controller
bool CMouse::Create(CPlayerInfo* thePlayerInfo)
{
	CController::Create(thePlayerInfo);
	cout << "CMouse::Create()" << endl;

	return false;
}


// Read from the controller
int CMouse::Read(const float deltaTime)
{
	CController::Read(deltaTime);
	//cout << "CMouse::Read()" << endl;

	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	UpdateMouseDelta((float)mouse_diff_x, (float)mouse_diff_y);

	Change(MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET));

	// if Mouse Buttons were activated, then act on them
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
		FirePrimary();
	else if (MouseController::GetInstance()->IsButtonPressed(MouseController::RMB))
		FireSecondary();

	return 0;
}
