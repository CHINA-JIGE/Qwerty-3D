/********************************************************

				Scene3 : main render process 
				(Qwerty Distortion)

*******************************************************/
#include "Qwerty3D-PC.h"

using namespace Noise3D;

void Qwerty::Scene_MainQwerty3dRendering::Init(Noise3D::IScene * pScene)
{
	//diffuse map
	m_pTexMgr->CreateTextureFromFile("../../../media/Earth.jpg", "Earth", true, 1024, 1024, false);
	m_pTexMgr->CreateTextureFromFile("../../../model/qwerty/arc18_filtered.PNG", "brick", false, 256, 256, false);
	m_pTexMgr->CreateTextureFromFile("../../../media/sky.jpg", "Universe", false, 256, 256, false);

	//------------------MESH INITIALIZATION----------------
	//---------------------init QWERTY scene----------------
	m_pScreenDescriptor = m_pMeshMgr->CreateMesh("screen");
	m_pModelLoader->LoadFile_OBJ(m_pScreenDescriptor, "../../../model/qwerty/AsusZenbook-Screen-XY.obj");
	//meshList.push_back(pScreenDescriptor);
	m_pScreenDescriptor->SetPosition(0, 0, 0);
	m_pScreenDescriptor->SetCullMode(NOISE_CULLMODE_NONE);
	m_pScreenDescriptor->SetShadeMode(NOISE_SHADEMODE_PHONG);


	m_pModelLoader = pScene->GetModelLoader();
	N_SceneLoadingResult res;
	m_pModelLoader->LoadFile_FBX("../../../model/qwerty/testRoom.fbx", res);
	for (auto & name : res.meshNameList)
	{
		IMesh* pMesh = m_pMeshMgr->GetMesh(name);
		mMeshList.push_back(pMesh);
		pMesh->SetCullMode(NOISE_CULLMODE_BACK);
		pMesh->SetShadeMode(NOISE_SHADEMODE_PHONG);
	}

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

	//qwerty distortion's param
	mQwertyDesc.pCamera = m_pCamera;
	mQwertyDesc.pScreenDescriptor = m_pScreenDescriptor;

}

void Qwerty::Scene_MainQwerty3dRendering::UpdateAndRender()
{
	mFunction_InputProcess();
	m_pRenderer->ClearBackground();

	//add to render list
	for (auto& pMesh : mMeshList)m_pRenderer->AddToRenderQueue(pMesh);
	m_pRenderer->SetActiveAtmosphere(m_pAtmos);
	m_pRenderer->AddToPostProcessList_QwertyDistortion(mQwertyDesc);

	m_pRenderer->Render();
	m_pRenderer->PresentToScreen();
}

void Qwerty::Scene_MainQwerty3dRendering::mFunction_InputProcess()
{
	//no respond to key press
}
