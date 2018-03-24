/******************************************************

						Main Application

*****************************************************/

#include "Qwerty3D-PC.h"

using namespace Noise3D;

Qwerty::MainApp::MainApp():
	mTimer(Noise3D::Ut::NOISE_TIMER_TIMEUNIT_MILLISECOND)
{

}

Qwerty::MainApp::~MainApp()
{
}

bool Qwerty::MainApp::Init3D(HINSTANCE hInstance, void(mainLoopFunc()))
{
	//get Root interface
	m_pRoot = GetRoot();

	//create a window (using default window creation function)
	HWND windowHWND;
	windowHWND = m_pRoot->CreateRenderWindow(1280, 800, L"Hahaha Render Window", hInstance);

	//initialize input engine (detection for keyboard and mouse input)
	mInputE.Initialize(hInstance, windowHWND);

	//register main loop function, call MainLoop to enter
	m_pRoot->SetMainLoopFunction(mainLoopFunc);

	//---------------------------------------
	const UINT bufferWidth = 1280;
	const UINT bufferHeight = 800;

	//init failed, exit
	if (!m_pRoot->Init())return false;

	//query pointer to IScene
	m_pScene = m_pRoot->GetScenePtr();

	//retrieve meshMgr and Create new mesh
	m_pRenderer = m_pScene->CreateRenderer(bufferWidth, bufferHeight, windowHWND);
	//m_pRenderer->SwitchToFullScreenMode();

	//Init every scenes
	mScene_WaitForConnection.Init(m_pScene);
	mScene_CamCalib.Init(m_pScene);
	mScene_MainQwertyRender.Init(m_pScene);
	mScene_DebugCamPos.Init(m_pScene, &mInputE);

	return true;
}

void Qwerty::MainApp::EnterMainLoop()
{
	m_pRoot->Mainloop();
}

void Qwerty::MainApp::UpdateAndRender()
{
	mTimer.NextTick();

	switch (mSceneState)
	{
	case Q_SCENE_STATE::WAITING_FOR_CONNECTION:
	{
		mScene_WaitForConnection.UpdateAndRender();
		break;
	}
	case Q_SCENE_STATE::CALIBRATING_CAMERA :
	{
		mScene_CamCalib.UpdateAndRender();
		break;
	}
	case Q_SCENE_STATE::QWERTY_3D_RENDERING:
	{
		mScene_MainQwertyRender.UpdateAndRender();
		break;
	}
	case Q_SCENE_STATE::DEBUGGING_CAMERA_PATH:
	{
		mScene_DebugCamPos.UpdateAndRender();
		break;
	}

	//quit main loop and terminate program
	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_ESCAPE))
	{
		IRoot* pRoot = Noise3D::GetRoot();
		pRoot->SetMainLoopStatus(NOISE_MAINLOOP_STATUS_QUIT_LOOP);
	}

	}

}

void Qwerty::MainApp::Cleanup()
{
	m_pRoot->ReleaseAll();
}

void Qwerty::MainApp::SetCurrentScene(Q_SCENE_STATE state)
{
	mSceneState = state;
}


