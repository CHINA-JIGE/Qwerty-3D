/********************************************************

				Scene4 : debugging camera path 
				(free roaming to watch historical pos)

*******************************************************/

#pragma once

namespace Qwerty
{
	class Scene_DebuggingCameraPos
	{
	public:

		void Init(Noise3D::IScene* pScene,Noise3D::Ut::IInputEngine* pInputE);

		void UpdateAndRender();

	private:

		void mFunction_InputProcess();

		Noise3D::IRenderer*			m_pRenderer;
		Noise3D::ICamera*				m_pCamera;
		Noise3D::IAtmosphere*		m_pAtmos;
		Noise3D::IGraphicObjectManager*	m_pGraphicObjMgr;
		Noise3D::IGraphicObject*	m_pChessboard;//points and lines
		Noise3D::Ut::IInputEngine* m_pRefInputE;
	};

}