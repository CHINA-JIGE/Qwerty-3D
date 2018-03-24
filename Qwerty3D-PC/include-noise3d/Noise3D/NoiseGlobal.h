
/***********************************************************************

                           h��NoiseGlobal

************************************************************************/


#pragma once

//#define NOISE_DLL_EXPORT /*_declspec(dllexport)*/

namespace Noise3D
{

	//g_cXXX for Global Const XXX 
	const  UINT g_cVBstride_Default = sizeof(N_DefaultVertex);		//VertexBuffer��ÿ��Ԫ�ص��ֽڿ��
	
	const  UINT g_cVBstride_Simple = sizeof(N_SimpleVertex);
	
	const  UINT g_cVBoffset = 0;				//VertexBuffer index offset ( 0 for start from the beginning)

	const UINT	g_VertexDesc_Default_ElementCount = 5;

	const UINT	g_VertexDesc_Simple_ElementCount = 3;

	//����Ⱦ��������سߴ�

	extern D3D_DRIVER_TYPE			g_Device_driverType;
	//
	extern D3D_FEATURE_LEVEL		g_Device_featureLevel;

	//��������
	extern D3D11_INPUT_ELEMENT_DESC g_VertexDesc_Default[g_VertexDesc_Default_ElementCount];

	extern D3D11_INPUT_ELEMENT_DESC g_VertexDesc_Simple[g_VertexDesc_Simple_ElementCount];

	//��������������������ȫ�ֽӿڡ�������������������

	extern ID3D11Device*					g_pd3dDevice11;

	extern ID3D11DeviceContext*	g_pImmediateContext;

	extern ID3DX11Effect*				g_pFX;//shader management framework's interface ,could be used by multiple classes

	//���㲼��
	extern ID3D11InputLayout*			g_pVertexLayout_Default;

	extern ID3D11InputLayout*			g_pVertexLayout_Simple;

	//��������������������ȫ�ֺ���������������������������

	extern /*_declspec(dllexport)*/ bool gFunc_IsPointInRect2D(NVECTOR2 v, NVECTOR2 vTopLeft, NVECTOR2 vBottomRight);

	extern /*_declspec(dllexport)*/ int	gFunc_GetCharAlignmentOffsetPixelY(UINT boundaryPxHeight, UINT charRealHeight, wchar_t inputChar);

	extern /*_declspec(dllexport)*/ std::string gFunc_GetFileFolderFromPath(std::string completeFilePath);

	extern	/*_declspec(dllexport)*/  std::string gFunc_GetFileNameFromPath(std::string completeFilePath);

	extern	/*_declspec(dllexport)*/  std::string gFunc_GetFileSubFixFromPath(std::string path);

	extern /*_declspec(dllexport)*/ float Lerp(float a, float b, float t);

	extern /*_declspec(dllexport)*/ NVECTOR3 Lerp(NVECTOR3 v1, NVECTOR3 v2, float t);

	extern /*_declspec(dllexport)*/ float Clamp(float val, float min, float max);

	extern /*_declspec(dllexport)*/ int	Clamp(int val, int min, int max);

	extern /*declspec(dllexport)*/ double Clamp(double val, double min, double max);

	extern /*_declspec(dllexport)*/ NVECTOR3 Clamp(const NVECTOR3& target, const NVECTOR3& min, const NVECTOR3& max);

}