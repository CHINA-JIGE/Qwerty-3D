/******************************************************

						Main Application

*****************************************************/

#include "Qwerty3D-PC.h"

using namespace Noise3D;

Qwerty::RenderModule::RenderModule():
	mTimer(Noise3D::Ut::NOISE_TIMER_TIMEUNIT_MILLISECOND)
{

}

Qwerty::RenderModule::~RenderModule()
{
}

bool Qwerty::RenderModule::Init3D(HINSTANCE hInstance, void(mainLoopFunc()))
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
	m_pMeshMgr = m_pScene->GetMeshMgr();
	m_pRenderer = m_pScene->CreateRenderer(bufferWidth, bufferHeight, windowHWND);
	m_pRenderer->SwitchToFullScreenMode();
	m_pCamera = m_pScene->GetCamera();
	m_pLightMgr = m_pScene->GetLightMgr();
	m_pMatMgr = m_pScene->GetMaterialMgr();
	m_pTexMgr = m_pScene->GetTextureMgr();
	m_pAtmos = m_pScene->GetAtmosphere();
	m_pGraphicObjMgr = m_pScene->GetGraphicObjMgr();

	//diffuse map
	m_pTexMgr->CreateTextureFromFile("../../../media/Earth.jpg", "Earth", true, 1024, 1024, false);
	m_pTexMgr->CreateTextureFromFile("../../../model/qwerty/arc18_filtered.PNG", "brick", false, 256, 256, false);

	m_pTexMgr->CreateTextureFromFile("../../../media/sky.jpg", "Universe", false, 256, 256, false);
	m_pTexMgr->CreateTextureFromFile("../../../media/bottom-right-conner-title.jpg", "BottomRightTitle", true, 0, 0, false);

	//create font texture
	m_pFontMgr = m_pScene->GetFontMgr();
	m_pFontMgr->CreateFontFromFile("../../../media/STXINWEI.ttf", "myFont", 24);
	m_pMyText_fps = m_pFontMgr->CreateDynamicTextA("myFont", "fpsLabel", "fps:000", 200, 100, NVECTOR4(0, 0, 0, 1.0f), 0, 0);
	m_pMyText_fps->SetTextColor(NVECTOR4(0, 0.3f, 1.0f, 0.5f));
	m_pMyText_fps->SetDiagonal(NVECTOR2(20, 20), NVECTOR2(170, 60));
	m_pMyText_fps->SetFont("myFont");
	m_pMyText_fps->SetBlendMode(NOISE_BLENDMODE_ALPHA);

	//---------------------INIT QWERTY----------------
	m_pScreenDescriptor = m_pMeshMgr->CreateMesh("screen");
	m_pModelLoader->LoadFile_OBJ(m_pScreenDescriptor, "../../../model/qwerty/AsusZenbook-Screen-XY.obj");
	//meshList.push_back(pScreenDescriptor);
	m_pScreenDescriptor->SetPosition(0, 0, 0);
	m_pScreenDescriptor->SetCullMode(NOISE_CULLMODE_NONE);
	m_pScreenDescriptor->SetShadeMode(NOISE_SHADEMODE_PHONG);

	//------------------MESH INITIALIZATION----------------
	m_pModelLoader = m_pScene->GetModelLoader();
	N_SceneLoadingResult res;
	m_pModelLoader->LoadFile_FBX("../../../model/qwerty/testRoom.fbx", res);

	for (auto & name : res.meshNameList)
	{
		IMesh* pMesh = m_pMeshMgr->GetMesh(name);
		mMeshList.push_back(pMesh);
		pMesh->SetCullMode(NOISE_CULLMODE_BACK);
		pMesh->SetShadeMode(NOISE_SHADEMODE_PHONG);
	}

	m_pGraphicObjBuffer->AddLine3D({ 0,0,0 }, { 50.0f,0,0 }, { 1.0f,0,0,1.0f }, { 1.0f,0,0,1.0f });
	m_pGraphicObjBuffer->AddLine3D({ 0,0,0 }, { 0,50.0f,0 }, { 0,1.0f,0,1.0f }, { 0,1.0f,0,1.0f });
	m_pGraphicObjBuffer->AddLine3D({ 0,0,0 }, { 0,0,50.0f }, { 0,0,1.0f,1.0f }, { 0,0,1.0f,1.0f });

	//----------------------------------------------------------

	m_pCamera->SetViewAngle_Radian(MATH_PI *0.4f, float(1.5f));
	m_pCamera->SetViewFrustumPlane(1.0f, 3000.f);
	m_pCamera->SetPosition(170.0f, 10.0f, -170.0f);
	m_pCamera->SetLookAt(0, 0, 0);


	m_pModelLoader->LoadSkyDome(m_pAtmos, "Universe", 4.0f, 2.0f);
	m_pAtmos->SetFogEnabled(false);
	m_pAtmos->SetFogParameter(50.0f, 100.0f, NVECTOR3(0, 0, 1.0f));

	m_pPointLight1 = m_pLightMgr->CreateDynamicPointLight("myPointLight1");
	N_PointLightDesc pointLightDesc;
	pointLightDesc.ambientColor = NVECTOR3(1.0, 1.0, 1.0);
	pointLightDesc.diffuseColor = NVECTOR3(0, 0, 0);
	pointLightDesc.specularColor = NVECTOR3(0, 0, 0);
	pointLightDesc.mAttenuationFactor = 0.0001f;
	pointLightDesc.mLightingRange = 1000.0f;
	pointLightDesc.mPosition = NVECTOR3(0, 0, -40.0f);
	pointLightDesc.specularIntensity = 1.0f;
	pointLightDesc.diffuseIntensity = 1.0f;
	m_pPointLight1->SetDesc(pointLightDesc);


	//bottom right
	m_pGraphicObjBuffer->AddRectangle(NVECTOR2(1100.0f, 750.0f), NVECTOR2(1280.0f, 800.0f), NVECTOR4(0.3f, 0.3f, 1.0f, 1.0f), "BottomRightTitle");
	m_pGraphicObjBuffer->SetBlendMode(NOISE_BLENDMODE_ALPHA);

	mQwertyDesc.pCamera = m_pCamera;
	mQwertyDesc.pScreenDescriptor = m_pScreenDescriptor;

	return true;
}

void Qwerty::RenderModule::EnterMainLoop()
{
	m_pRoot->Mainloop();
}

void Qwerty::RenderModule::Render()
{
	mFunction_InputProcess();
	m_pRenderer->ClearBackground();
	mTimer.NextTick();

	//update fps lable
	std::stringstream tmpS;
	tmpS << "fps :" << mTimer.GetFPS() << std::endl;
	m_pMyText_fps->SetTextAscii(tmpS.str());


	//add to render list
	for (auto& pMesh : mMeshList)m_pRenderer->AddToRenderQueue(pMesh);
	m_pRenderer->AddToRenderQueue(m_pGraphicObjBuffer);
	m_pRenderer->AddToRenderQueue(m_pMyText_fps);
	m_pRenderer->SetActiveAtmosphere(m_pAtmos);

	//pRenderer->AddToPostProcessList_QwertyDistortion(qwertyDesc);

	//render
	m_pRenderer->Render();

	//present
	m_pRenderer->PresentToScreen();
}

void Qwerty::RenderModule::Cleanup()
{
	m_pRoot->ReleaseAll();
}


/************************************************
							PRIVATE
************************************************/

void Qwerty::RenderModule::mFunction_InputProcess()
{
	mInputE.Update();

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_A))
	{
		m_pCamera->fps_MoveRight(-0.5f, FALSE);
	}

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_D))
	{
		m_pCamera->fps_MoveRight(0.5f, FALSE);
	}

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_W))
	{
		m_pCamera->fps_MoveForward(0.5f, FALSE);
	}

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_S))
	{
		m_pCamera->fps_MoveForward(-0.5f, FALSE);
	}

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_SPACE))
	{
		m_pCamera->fps_MoveUp(0.5f);
	}

	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_LCONTROL))
	{
		m_pCamera->fps_MoveUp(-0.5f);
	}

	if (mInputE.IsMouseButtonPressed(Ut::NOISE_MOUSEBUTTON_LEFT))
	{
		m_pCamera->RotateY_Yaw((float)mInputE.GetMouseDiffX() / 200.0f);
		m_pCamera->RotateX_Pitch((float)mInputE.GetMouseDiffY() / 200.0f);
	}

	//quit main loop and terminate program
	if (mInputE.IsKeyPressed(Ut::NOISE_KEY_ESCAPE))
	{
		m_pRoot->SetMainLoopStatus(NOISE_MAINLOOP_STATUS_QUIT_LOOP);
	}

}
