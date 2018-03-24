/******************************************************

				Scene1 : prompt text of "waiting"

*****************************************************/
#include "Qwerty3D-PC.h"

using namespace Noise3D;
void Qwerty::Scene_WaitForConnection::Init(Noise3D::IScene* pScene)
{
	m_pRenderer = pScene->GetRenderer();
	m_pFontMgr = pScene->GetFontMgr();

	//create a font for dynamic text
	const std::string textFontName = "promptFont1";
	m_pFontMgr->CreateFontFromFile("../../../media/STXINWEI.ttf", textFontName, 24);

	//create dynamic text
	m_pPromptText = m_pFontMgr->CreateDynamicTextA(
		textFontName,
		"promptText_waitConnection",
		"Waiting For Connection. Please Open Qwerty 3D Mobile App",
		600, 200,NVECTOR4(1.0f,1.0f,1.0f,1.0f),0,2);

	//move to center
	uint32_t w = m_pRenderer->GetBackBufferWidth();
	uint32_t h = m_pRenderer->GetBackBufferHeight();
	m_pPromptText->SetCenterPos(float(w) / 2.0f, float(h) / 2.0f);
}


void Qwerty::Scene_WaitForConnection::UpdateAndRender()
{
	//fps control
	Sleep(20);
	mFunction_InputProcess();

	//clear
	m_pRenderer->ClearBackground();

	//oscillate text color
	static float textColorPhase = 0.0f;
	textColorPhase += 0.01f;
	m_pPromptText->SetTextColor({ abs(sinf(textColorPhase)), abs(sinf(textColorPhase)), abs(sinf(textColorPhase)), 1.0f });

	//render
	m_pRenderer->AddToRenderQueue(m_pPromptText);
	m_pRenderer->Render();
	m_pRenderer->PresentToScreen();
}

/******************************************************
						 P R I V A T E
*****************************************************/
void Qwerty::Scene_WaitForConnection::mFunction_InputProcess()
{
	//no respond to key press
}
