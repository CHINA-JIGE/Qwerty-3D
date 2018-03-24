
#include "Qwerty3D-PC.h"

Qwerty::MainApp mainApp;
Qwerty::NetworkModule networkModule;
void Mainloop();

//Main Entry
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	mainApp.Init3D(hInstance,Mainloop);

	networkModule.Init(
		"tcp://39.108.118.166:1883",
		"JigeClient-PC",
		"FuckingTestMqtt");

	bool isConnected =networkModule.Connect();

	//enter main loop (until IRoot::MainLoopStatus change to QUIT_LOOP)
	mainApp.EnterMainLoop();

	//cleanup and exit
	networkModule.Disconnect();

	mainApp.Cleanup();

	return 0;
}

void Mainloop()
{
	mainApp.UpdateAndRender();
}