/********************************************************

				Scene2 : Zhang's[00] chessboard 
				camera calibration

*******************************************************/
#include "Qwerty3D-PC.h"

void Qwerty::Scene_CameraCalibration::Init(Noise3D::IScene * pScene)
{
	m_pRenderer = pScene->GetRenderer();
	m_pGraphicObjMgr = pScene->GetGraphicObjMgr();
}

void Qwerty::Scene_CameraCalibration::UpdateAndRender()
{
	m_pRenderer->ClearBackground({ 1.0f,1.0f,1.0f,1.0f });

	m_pRenderer->Render();
	m_pRenderer->PresentToScreen();
}

void Qwerty::Scene_CameraCalibration::mFunction_InputProcess()
{

}
