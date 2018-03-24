/********************************************************

				Scene4 : debugging camera path 
				(free roaming to watch historical pos)

*******************************************************/
#include "Qwerty3D-PC.h"

using namespace Noise3D;

void Qwerty::Scene_DebuggingCameraPos::Init(Noise3D::IScene* pScene, Noise3D::Ut::IInputEngine* pInputE)
{
	m_pRenderer = pScene->GetRenderer();
	m_pAtmos = pScene->GetAtmosphere();
	m_pCamera = pScene->GetCamera();
	m_pGraphicObjMgr = pScene->GetGraphicObjMgr();
	m_pChessboard = m_pGraphicObjMgr->CreateGraphicObj("scene4GO");
	m_pRefInputE = pInputE;
}

void Qwerty::Scene_DebuggingCameraPos::UpdateAndRender()
{
	mFunction_InputProcess();
	m_pRenderer->ClearBackground();

	//add to render list
	m_pRenderer->SetActiveAtmosphere(m_pAtmos);
	m_pRenderer->AddToRenderQueue(m_pChessboard);

	m_pRenderer->Render();
	m_pRenderer->PresentToScreen();
}

/********************************************************
											PRIVATE
*******************************************************/
void Qwerty::Scene_DebuggingCameraPos::mFunction_InputProcess()
{
	m_pRefInputE->Update();

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_A))
	{
		m_pCamera->fps_MoveRight(-0.5f, FALSE);
	}

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_D))
	{
		m_pCamera->fps_MoveRight(0.5f, FALSE);
	}

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_W))
	{
		m_pCamera->fps_MoveForward(0.5f, FALSE);
	}

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_S))
	{
		m_pCamera->fps_MoveForward(-0.5f, FALSE);
	}

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_SPACE))
	{
		m_pCamera->fps_MoveUp(0.5f);
	}

	if (m_pRefInputE->IsKeyPressed(Ut::NOISE_KEY_LCONTROL))
	{
		m_pCamera->fps_MoveUp(-0.5f);
	}

	if (m_pRefInputE->IsMouseButtonPressed(Ut::NOISE_MOUSEBUTTON_LEFT))
	{
		m_pCamera->RotateY_Yaw((float)m_pRefInputE->GetMouseDiffX() / 200.0f);
		m_pCamera->RotateX_Pitch((float)m_pRefInputE->GetMouseDiffY() / 200.0f);
	}

}
