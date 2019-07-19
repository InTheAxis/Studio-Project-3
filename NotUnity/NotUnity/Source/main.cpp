#include "Application.h"

int main()
{
	Application app;
	app.Create();
	app.Run(); //main loop
	app.Exit();
	return 0;
}