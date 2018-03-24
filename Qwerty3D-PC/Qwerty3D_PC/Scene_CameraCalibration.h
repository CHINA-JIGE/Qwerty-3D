/********************************************************

				Scene2 : Zhang's[00] chessboard 
				camera calibration

*******************************************************/

#pragma once

namespace Qwerty
{

	class Scene_CameraCalibration
	{
	public:

		void Init(Noise3D::IScene* pScene);

		void UpdateAndRender();

	private:

		void mFunction_InputProcess();

		Noise3D::IRenderer*		m_pRenderer;
		Noise3D::IGraphicObjectManager*	m_pGraphicObjMgr;
		Noise3D::IGraphicObject*	m_pChessboard;
	};

}