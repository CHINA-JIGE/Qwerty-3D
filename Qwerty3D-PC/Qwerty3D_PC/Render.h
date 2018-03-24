/******************************************************

			Main render functions/ Main loop

*****************************************************/

#pragma once

namespace Qwerty
{
	class RenderModule
	{
	public:

		RenderModule();

		~RenderModule();

		bool Init3D(HINSTANCE hInstance, void(mainLoopFunc()));

		void EnterMainLoop();

		void Render();

		void Cleanup();

	private:

		//deal with device input like keyboard/mouse
		void	mFunction_InputProcess();


		Noise3D::IRoot*			m_pRoot;
		Noise3D::IRenderer*			m_pRenderer;
		Noise3D::IScene*					m_pScene;
		Noise3D::ICamera*				m_pCamera;
		Noise3D::IAtmosphere*		m_pAtmos;
		Noise3D::IModelLoader*		m_pModelLoader;
		Noise3D::IMeshManager*	m_pMeshMgr;
		std::vector<Noise3D::IMesh*>mMeshList;
		Noise3D::IMesh*						m_pScreenDescriptor;
		Noise3D::IMaterialManager*	m_pMatMgr;
		Noise3D::ITextureManager*	m_pTexMgr;
		Noise3D::IGraphicObjectManager*	m_pGraphicObjMgr;
		Noise3D::IGraphicObject*	m_pGraphicObjBuffer;
		Noise3D::ILightManager*	m_pLightMgr;
		Noise3D::IDirLightD*			m_pDirLight1;
		Noise3D::IPointLightD*		m_pPointLight1;
		Noise3D::IFontManager*		m_pFontMgr;
		Noise3D::IDynamicText*		m_pMyText_fps;
		Noise3D::N_PostProcesQwertyDistortionDesc	mQwertyDesc;
		Noise3D::Ut::ITimer				mTimer;
		Noise3D::Ut::IInputEngine	mInputE;

	};
}
