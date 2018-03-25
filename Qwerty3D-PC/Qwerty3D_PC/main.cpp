
#include "Qwerty3D-PC.h"

void Mainloop();
using namespace Qwerty;

//Main Entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	//init 3D rendering
	gMainApp.Init3D(hInstance,Mainloop);

	//connect to remote mobile head tracking app with MQTT
	gMainApp.InitConnection();

	//enter main loop (until IRoot::MainLoopStatus change to QUIT_LOOP)
	gMainApp.EnterMainLoop();

	gMainApp.Cleanup();

	return 0;
}

void Mainloop()
{
	gMainApp.UpdateAndRender();
}