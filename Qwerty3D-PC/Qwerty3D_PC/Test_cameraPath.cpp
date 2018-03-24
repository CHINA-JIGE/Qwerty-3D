
#include "Qwerty3D-PC.h"

Qwerty::RenderModule renderModule;
Qwerty::NetworkModule networkModule(
	"tcp://39.108.118.166:1883", 
	"WindyIceXXX",
	"WindyIce_linearAcc");
void Mainloop();

//Main Entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	renderModule.Init3D(hInstance,Mainloop);

	bool isConnected =networkModule.InitConnection();

	//enter main loop
	renderModule.EnterMainLoop();

	renderModule.Cleanup();

	//..
	return 0;
}

void Mainloop()
{
	renderModule.Render();
}