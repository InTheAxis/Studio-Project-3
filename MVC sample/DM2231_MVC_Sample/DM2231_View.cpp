#include "DM2231_View.h"

DM2231_View::DM2231_View(DM2231_Model* theModel)
{
	this->theModel = theModel;

	theMouseInfo.Init();
	m_iWindows_Width = 1; 
	m_iWindows_Height = 1;

	m_bActive = true; // Window Active Flag Set To TRUE By Default
	m_bFullScreen = false; // Fullscreen Flag Set To Fullscreen Mode By Default

	hDC=NULL;
	hRC=NULL;
	hWnd=NULL;

	for (int i=0; i<256; i++)
	{
		keys[i] = false;
	}
}

DM2231_View::~DM2231_View(void)
{
	this->theModel = NULL;
}

// Draw the view
BOOL DM2231_View::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
	glLoadIdentity(); // Reset The Current Modelview Matrix

	glTranslatef(-1.5f,0.0f,-6.0f); // Move Left 1.5 Units And Into The Screen 6.0
	glRotatef(rtri,0.0f,1.0f,0.0f); // Rotate The Triangle On The Y axis ( NEW )
	glBegin(GL_TRIANGLES); // Start Drawing A Triangle
		glColor3f(1.0f,0.0f,0.0f); // Set Top Point Of Triangle To Red
		glVertex3f( 0.0f, 1.0f, 0.0f); // First Point Of The Triangle
		glColor3f(0.0f,1.0f,0.0f); // Set Left Point Of Triangle To Green
		glVertex3f(-1.0f,-1.0f, 0.0f); // Second Point Of The Triangle
		glColor3f(0.0f,0.0f,1.0f); // Set Right Point Of Triangle To Blue
		glVertex3f( 1.0f,-1.0f, 0.0f); // Third Point Of The Triangle
	glEnd(); // Done Drawing The Triangle

	glLoadIdentity(); // Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-6.0f); // Move Right 1.5 Units And Into The Screen 6.0
	glRotatef(rquad,1.0f,0.0f,0.0f); // Rotate The Quad On The X axis ( NEW )
	glColor3f(0.5f,0.5f,1.0f); // Set The Color To Blue One Time Only

	glBegin(GL_QUADS); // Draw A Quad
		glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
	glEnd(); // Done Drawing The Quad

	rtri+=0.2f; // Increase The Rotation Variable For The Triangle ( NEW )
	rquad-=0.15f; // Decrease The Rotation Variable For The Quad ( NEW )

	SwapBuffers(hDC); // Swap Buffers (Double Buffering)

	return TRUE; // Keep Going
}

int DM2231_View::InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
	glClearDepth(1.0f); // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
	return TRUE; // Initialization Went OK
}

GLvoid DM2231_View::ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0) // Prevent A Divide By Zero By
	{
		height=1; // Making Height Equal One
	}

	m_iWindows_Width = width; 
	m_iWindows_Height = height;

	glViewport(0,0,width,height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}

GLvoid DM2231_View::KillGLWindow(GLvoid) // Properly Kill The Window
{
	if (m_bFullScreen) // Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0); // If So Switch Back To The Desktop
		ShowCursor(TRUE); // Show Mouse Pointer
	}

	if (hRC) // Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL)) // Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC)) // Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL; // Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC)) // Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL; // Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd)) // Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL; // Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance)) // Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL; // Set hInstance To NULL
	}
}

/* This Code Creates Our OpenGL Window. Parameters Are: *
* title - Title To Appear At The Top Of The Window *
* width - Width Of The GL Window Or Fullscreen Mode *
* height - Height Of The GL Window Or Fullscreen Mode *
* bits - Number Of Bits To Use For Color (8/16/24/32) *
* fullscreenflag - Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE) */

BOOL DM2231_View::CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint PixelFormat; // Holds The Results After Searching For A Match
	WNDCLASS wc; // Windows Class Structure
	DWORD dwExStyle; // Window Extended Style
	DWORD dwStyle; // Window Style
	RECT WindowRect; // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0; // Set Left Value To 0
	WindowRect.right=(long)width; // Set Right Value To Requested Width
	WindowRect.top=(long)0; // Set Top Value To 0
	WindowRect.bottom=(long)height; // Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL); // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC) WndProc; // WndProc Handles Messages
	wc.cbClsExtra = 0; // No Extra Window Data
	wc.cbWndExtra = 0; // No Extra Window Data
	wc.hInstance = hInstance; // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load The Arrow Pointer
	wc.hbrBackground = NULL; // No Background Required For GL
	wc.lpszMenuName = NULL; // We Don't Want A Menu
	wc.lpszClassName = "OpenGL"; // Set The Class Name

	if (!RegisterClass(&wc)) // Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	if (m_bFullScreen) // Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings; // Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings)); // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings); // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width; // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height; // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits; // Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results. NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options. Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				m_bFullScreen=false; // Windowed Mode Selected. Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE; // Return FALSE
			}
		}
	}

	if (m_bFullScreen) // Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW; // Window Extended Style
		dwStyle=WS_POPUP; // Windows Style
		ShowCursor(FALSE); // Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW; // Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx( dwExStyle, // Extended Style For The Window
		"OpenGL", // Class Name
		title, // Window Title
		dwStyle | // Defined Window Style
		WS_CLIPSIBLINGS | // Required Window Style
		WS_CLIPCHILDREN, // Required Window Style
		0, 0, // Window Position
		WindowRect.right-WindowRect.left, // Calculate Window Width
		WindowRect.bottom-WindowRect.top, // Calculate Window Height
		NULL, // No Parent Window
		NULL, // No Menu
		hInstance, // Instance
		NULL))) // Dont Pass Anything To WM_CREATE
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	static PIXELFORMATDESCRIPTOR pfd= // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1, // Version Number
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER, // Must Support Double Buffering
		PFD_TYPE_RGBA, // Request An RGBA Format
		bits, // Select Our Color Depth
		0, 0, 0, 0, 0, 0, // Color Bits Ignored
		0, // No Alpha Buffer
		0, // Shift Bit Ignored
		0, // No Accumulation Buffer
		0, 0, 0, 0, // Accumulation Bits Ignored
		16, // 16Bit Z-Buffer (Depth Buffer)
		0, // No Stencil Buffer
		0, // No Auxiliary Buffer
		PFD_MAIN_PLANE, // Main Drawing Layer
		0, // Reserved
		0, 0, 0 // Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd))) // Did We Get A Device Context?
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))) // Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd)) // Are We Able To Set The Pixel Format?
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC))) // Are We Able To Get A Rendering Context?
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC)) // Try To Activate The Rendering Context
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (long)this);

	ShowWindow(hWnd,SW_SHOW); // Show The Window
	UpdateWindow(hWnd);
	SetForegroundWindow(hWnd); // Slightly Higher Priority
	SetFocus(hWnd); // Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height); // Set Up Our Perspective GL Screen

	if (!InitGL()) // Initialize Our Newly Created GL Window
	{
		KillGLWindow(); // Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE; // Return FALSE
	}

	// Store the width and height
	m_iWindows_Width = width; 
	m_iWindows_Height = height;

	return TRUE; // Success
}

LRESULT CALLBACK DM2231_View::MsgProc( HWND hWnd, // Handle For This Window
									  UINT uMsg, // Message For This Window
									  WPARAM wParam, // Additional Message Information
									  LPARAM lParam) // Additional Message Information
{
	switch (uMsg) // Check For Windows Messages
	{
	case WM_ACTIVATE: // Watch For Window Activate Message
		{
			if (!HIWORD(wParam)) // Check Minimization State
			{
				m_bActive=TRUE; // Program Is Active
			}
			else
			{
				m_bActive=FALSE; // Program Is No Longer Active
			}

			return 1; // Return To The Message Loop
		}

	case WM_SYSCOMMAND: // Intercept System Commands
		{
			switch (wParam) // Check System Calls
			{
			case SC_SCREENSAVE: // Screensaver Trying To Start?
			case SC_MONITORPOWER: // Monitor Trying To Enter Powersave?
				return 1; // Prevent From Happening
			}
			break; // Exit
		}

	case WM_CLOSE: // Did We Receive A Close Message?
		{
			PostQuitMessage(0); // Send A Quit Message
			return 1; // Jump Back
		}

	case WM_KEYDOWN: // Is A Key Being Held Down?
		{
			keys[wParam] = TRUE; // If So, Mark It As TRUE
			return 1; // Jump Back
		}

	case WM_KEYUP: // Has A Key Been Released?
		{
			keys[wParam] = FALSE; // If So, Mark It As FALSE
			return 1; // Jump Back
		}

	case WM_SIZE: // Resize The OpenGL Window
		{
			// ReSizeGLScene(LOWORD(lParam),HIWORD(lParam)); // LoWord=Width, HiWord=Height
			return 1; // Jump Back
		}
	case WM_MOUSEMOVE:
		{
			theMouseInfo.SetMousePos( LOWORD(lParam), HIWORD(lParam) );
			int diffX = theMouseInfo.GetDiff_X();
			int diffY = theMouseInfo.GetDiff_Y();

			//camera->UpdateCamera( diffX, diffY);

			//Checking mouse boundary
			if  (theMouseInfo.x > m_iWindows_Width-20 || theMouseInfo.x < 20)
			{
				theMouseInfo.x = (m_iWindows_Width >> 1);
				SetCursorPos( theMouseInfo.x, theMouseInfo.y );
			}
			if (theMouseInfo.y > m_iWindows_Height-20 || theMouseInfo.y < 20)
			{
				theMouseInfo.y = (m_iWindows_Height >> 1);
				SetCursorPos( theMouseInfo.x, theMouseInfo.y );
			}
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


LRESULT CALLBACK DM2231_View::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Recover the pointer to our class, don't forget to type cast it back

	DM2231_View* winptr = (DM2231_View*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	// Check if the pointer is NULL and call the Default WndProc

	if (winptr == NULL)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	} else
	{
		// Call the Message Handler for my class (MsgProc in my case)
		return winptr->MsgProc(hwnd, message, wParam, lParam);
	}
}
// Check if the player wants to quit the game
bool DM2231_View::isQuitGame(void)
{
	if (keys[VK_ESCAPE])				// Was ESC Pressed?
		return true;
	else
		return false;
}

// Check if the player wants to play the game in Full Screen
bool DM2231_View::runFullScreen(void)
{
	if (keys[VK_F1])
		return true;
	else
		return false;
}

// Toggle the game to be fullscreen or otherwise
bool DM2231_View::toggleFullScreen(void)
{
	keys[VK_F1]=FALSE;					// If So Make Key FALSE
	m_bFullScreen=!m_bFullScreen;				// Toggle Fullscreen / Windowed Mode

	KillGLWindow();						// Kill Our Current Window
	// Recreate Our OpenGL Window
	if (!CreateGLWindow("DM2231 Game Development Techniques",800,600,16))
	{
		return false;						// Quit If Window Was Not Created
	}

	return true;
}

// Set the fullscreen display status
void DM2231_View::setFullScreen(bool m_bFullScreen)
{
	this->m_bFullScreen = m_bFullScreen;
}
