/******************************************************

				Scene1 : prompt text of "waiting"

*****************************************************/

#pragma once

namespace Qwerty
{

	class Scene_WaitForConnection
	{
	public:

		void Init(Noise3D::IScene* pScene);

		void UpdateAndRender();

	private:

		void mFunction_InputProcess();

		Noise3D::IFontManager*	m_pFontMgr;
		Noise3D::IRenderer*		m_pRenderer;
		Noise3D::IDynamicText*	m_pPromptText;
	};

}