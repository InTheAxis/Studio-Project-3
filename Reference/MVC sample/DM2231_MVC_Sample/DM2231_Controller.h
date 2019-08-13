#pragma once

#include <stdio.h>

#include "DM2231_Model.h"
#include "DM2231_View.h"

class DM2231_Controller
{
public:
	DM2231_Controller(DM2231_Model* theModel, DM2231_View* theView);
	~DM2231_Controller(void);

	// Get the status of the stop game boolean flag
	BOOL RunMainLoop(void);

	// Process input from I/O devices
	bool ProcessInput(void);

	inline void setHandle(HWND hwnd) { handle = hwnd; }

	// Handle to the model
	DM2231_Model* theModel;

	// Handle to the view
	DM2231_View* theView;

protected:
	HWND handle;                                            // window handle to map window to controller
	bool Init();

	bool m_bContinueLoop;


};

