/********************************************************

				Scene3 : main render process 
				(Qwerty Distortion)

*******************************************************/

#pragma once

namespace Qwerty
{
	class Scene_MainQwerty3dRendering
	{
	public:

		void Init(Noise3D::IScene* pScene);

		void UpdateAndRender();

	private:

		void mFunction_InputProcess();

		Noise3D::IRenderer*			m_pRenderer;
		Noise3D::ICamera*				m_pCamera;
		Noise3D::IAtmosphere*		m_pAtmos;
		Noise3D::IModelLoader*		m_pModelLoader;
		Noise3D::IMeshManager*	m_pMeshMgr;
		std::vector<Noise3D::IMesh*>mMeshList;
		Noise3D::IMesh*						m_pScreenDescriptor;
		Noise3D::IMaterialManager*	m_pMatMgr;
		Noise3D::ITextureManager*	m_pTexMgr;

		Noise3D::ILightManager*	m_pLightMgr;
		Noise3D::IPointLightD*		m_pPointLight1;
		Noise3D::N_PostProcesQwertyDistortionDesc	mQwertyDesc;
	};

}