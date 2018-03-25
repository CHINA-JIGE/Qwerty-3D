/******************************************************

						Main Application

*****************************************************/

#include "Qwerty3D-PC.h"

using namespace Noise3D;

Qwerty::MainApp Qwerty::gMainApp;

Qwerty::MainApp::MainApp():
	mTimer(Noise3D::Ut::NOISE_TIMER_TIMEUNIT_MILLISECOND),
	mSceneState(Qwerty::Q_SCENE_STATE::WAITING_FOR_CONNECTION)
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
	windowHWND = m_pRoot->CreateRenderWindow(1280, 800, L"Qwerty 3D PC¶Ë³ÌÐò v0.1", hInstance);

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

bool Qwerty::MainApp::InitConnection()
{
	//(2018.3.25)currently the target server is an Azure Cloud Server of PeiWei Chang)
	mNetworkModule.Init(
		"tcp://39.108.118.166:1883",
		"JigeClient-PC",
		"FuckingTestMqtt");

	bool isConnected = mNetworkModule.Connect();

	return isConnected;
}

void Qwerty::MainApp::EnterMainLoop()
{
	m_pRoot->Mainloop();
}

//execution of state machine
//(state transition happens in many places, like MainApp::OnMessage(), etc.)
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
		case Q_SCENE_STATE::CONNECTED_PROMPT:
		{
			//same as 'waiting for connection', just the text content changes
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
	//disconnect
	mNetworkModule.Disconnect();

	m_pRoot->ReleaseAll();
}

void Qwerty::MainApp::SetSceneState(Q_SCENE_STATE state)
{
	mSceneState = state;
}

Qwerty::Q_SCENE_STATE Qwerty::MainApp::GetSceneState()
{
	return mSceneState;
}

void Qwerty::MainApp::OnMessage(std::string && msg)
{
	//called-back by Network Module's overrided 'message_arrive'
	//Parse msg and do corresponding task (mostly scene state transition)

	//command format:
	//every command start with exactly 4 char (and relieve the burden of parsing- -)
	/*1: connection establishment (from qwerty app):

	 cnet

	*/
	/*2: camera calibration completed.

	clbc

	*/

	if (msg.size() < 4)return;

	std::string cmdStr = msg.substr(0, 4);

	switch (mSceneState)
	{
	case Q_SCENE_STATE::WAITING_FOR_CONNECTION:
	{
		if (cmdStr == "cnet")//connection establishment
			MainApp::SetSceneState(Q_SCENE_STATE::CONNECTED_PROMPT);
		break;
	}

	case Q_SCENE_STATE::CALIBRATING_CAMERA:
	{
		if (cmdStr == "q3mr")//calibration complete, qwerty 3D main rendering
		{
			MainApp::SetSceneState(Q_SCENE_STATE::QWERTY_3D_RENDERING);
			break;
		}

		if (cmdStr == "cpdb")//calibration complete, camera path debug
		{
			MainApp::SetSceneState(Q_SCENE_STATE::DEBUGGING_CAMERA_PATH);
			break;
		}

		break;
	}

	case Q_SCENE_STATE::QWERTY_3D_RENDERING:
	{
		if (cmdStr == "cpos")//camerea position
		{
			//Update cam pos accordingly 

		}
		break;
	}

	case Q_SCENE_STATE::DEBUGGING_CAMERA_PATH:
	{
		if (cmdStr == "cpos")//camerea position
		{
			//Update cam pos accordingly 
		}

		break;
	}
	}
}


