/******************************************************

			Main render functions/ Main loop

*****************************************************/

#pragma once

namespace Qwerty
{

	//scene states
	enum Q_SCENE_STATE
	{
		WAITING_FOR_CONNECTION,	//prompt for 'waiting for connection'
		CONNECTED_PROMPT,	//prompt for 'waiting for connection'
		CALIBRATING_CAMERA,			//camera calibration using chessboard
		QWERTY_3D_RENDERING,		//render with Qwerty Distortion
		DEBUGGING_CAMERA_PATH//free roaming of camera path
	};

	class MainApp
	{
	public:

		MainApp();

		~MainApp();

		bool Init3D(HINSTANCE hInstance, void(mainLoopFunc()));

		bool InitConnection();

		void EnterMainLoop();

		void UpdateAndRender();

		void Cleanup();

		//state transition
		void SetSceneState(Q_SCENE_STATE state);

		Qwerty::Q_SCENE_STATE GetSceneState();

		//parse messages and deal with state transition
		void OnMessage(std::string&& msg);

	private:

		//the scene state transition is highly dependent on the Mobile App
		//so state transition happens on Remote message received by MQTT network module
		Q_SCENE_STATE mSceneState;
		Scene_WaitForConnection mScene_WaitForConnection;
		Scene_CameraCalibration	mScene_CamCalib;
		Scene_MainQwerty3dRendering mScene_MainQwertyRender;
		Scene_DebuggingCameraPos mScene_DebugCamPos;

		Qwerty::NetworkModule mNetworkModule;

		Noise3D::IRoot*				m_pRoot;
		Noise3D::IScene*				m_pScene;
		Noise3D::IRenderer*		m_pRenderer;

		Noise3D::Ut::ITimer				mTimer;
		Noise3D::Ut::IInputEngine	mInputE;

	};

	extern Qwerty::MainApp gMainApp;

}
