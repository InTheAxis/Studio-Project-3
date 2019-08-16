#include "Application.h"

int main()
{
	Application app;
	app.Create();
	app.Run(); //main loop
	app.Exit();
#ifdef _DEBUG	
	system("pause");
#endif
	return 0;
}