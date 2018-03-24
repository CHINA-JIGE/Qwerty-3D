
/***********************************************************************

                           �ࣺNOISE Engine

			��������Ҫ�������ȫ�ֽӿںͱ�������ʼ����

************************************************************************/

#include "Noise3D.h"

using namespace Noise3D;

/*_declspec(dllexport)*/ IRoot* Noise3D::GetRoot()
{
	class IRootCreation :public IFactory<IRoot>
	{
	public:

		IRootCreation() :IFactory<IRoot>(1) {};

		~IRootCreation() {}

		IRoot* GetRoot()
		{
			static int rootCount = 0;
			//if a Root was never created, create one
			if (rootCount == 0)
			{
				rootCount++;
				m_pRoot = IFactory<IRoot>::CreateObject("Root");
				return m_pRoot;
			}
			else
			{
				//return the existed IRoot
				return m_pRoot;
			}

		};

	private:
		friend class IFactory<IRoot>;

		IRoot* m_pRoot;
	};

	static IRootCreation rootCreationFactory;
	static IRoot* ptr = rootCreationFactory.GetRoot();
	return  ptr;
};



//Constructor
IRoot::IRoot() :
	IFactory<IScene>(1)
{
	mRenderWindowTitle = L"Noise 3D - Render Window";
	m_pMainLoopFunction = nullptr;
	mMainLoopStatus = NOISE_MAINLOOP_STATUS_BUSY;
	mMainBackBufferHeight = 0;
	mMainBackBufferWidth = 0;
}

IRoot::~IRoot()
{
	m_pMainLoopFunction = nullptr;
	ReleaseAll();
}

IScene* IRoot::GetScenePtr()
{
	const N_UID sceneUID = "myScene";
	//first time to get a ScenePtr,Create one
	if (IFactory<IScene>::GetObjectCount() == 0)
	{
		IFactory<IScene>::CreateObject(sceneUID);
	}
	return IFactory<IScene>::GetObjectPtr(sceneUID);
}

HWND IRoot::CreateRenderWindow(UINT pixelWidth, UINT pixelHeight, LPCWSTR windowTitle, HINSTANCE hInstance)
{

	WNDCLASS wndclass;
	HWND outHWND;//���
	mRenderWindowHINSTANCE = hInstance;
	mRenderWindowTitle = windowTitle;
	mRenderWindowClassName = L"Noise Engine Render Window";

	//������ע��
	if (mFunction_InitWindowClass(&wndclass) == false)
	{
		ERROR_MSG("Window Class ����ʧ��");
		return false;
	};

	//��������

	outHWND = mFunction_InitWindow(pixelWidth,pixelHeight);
	if (outHWND == 0)
	{
		ERROR_MSG("���崴��ʧ��");
		return false;
	};

	return outHWND;
};

bool IRoot::Init()
{
	HRESULT hr = S_OK;

	//Ӳ����������
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	//HAL
		D3D_DRIVER_TYPE_REFERENCE,	//REF device
		D3D_DRIVER_TYPE_WARP,		//Windows Advanced Rasterization Platform, D3D10.1 only
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//D3D feature levels (9_1,9_29_3, 10_0 ,10_1,11_0,11_1etc.)
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//flag of device creation:
	UINT createDeviceFlags = 0;
#ifdef _DEBUG	
	//device debug mode, enable outputing d3d info
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//���оٳ�����Ӳ����ʽ ���Գ�ʼ�� ֱ���ɹ�
	UINT driverTypeIndex = 0;
	for (driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		//D3D_DRIVER_TYPE 
		g_Device_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(
			NULL,				//null��ʾʹ������ʾ��
			g_Device_driverType,		//�������� HAL/REF
			NULL,
			createDeviceFlags,//createDeviceFlags,	//�ǲ��ǵ���ģʽ	
			featureLevels,		//��D3Dѡ������Եİ汾
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&g_pd3dDevice11,		//����D3D�豸ָ��
			&g_Device_featureLevel,	//��������ʹ�õ����Եİ汾
			&g_pImmediateContext//����
		);
		//�����ɹ��˾Ͳ��ü������Դ���
		if (SUCCEEDED(hr))
		{
			break;
		};
	};
	//���Դ����豸ʧ��
	HR_DEBUG(hr, "IRoot : D3D Device Creation failed /n DriverType"+std::to_string(g_Device_driverType));

	
	if(!mFunction_CreateEffectFromMemory())return false;

	return true;

};

void IRoot::ReleaseAll()
{

	IScene* pScene = GetScenePtr();
	pScene->ReleaseAllChildObject();
	IFactory<IScene>::DestroyAllObject();//delete the only scene

	ReleaseCOM(g_pVertexLayout_Default);
	ReleaseCOM(g_pVertexLayout_Simple);
	ReleaseCOM(g_pImmediateContext);
	ReleaseCOM(g_pFX);
	//check live object
#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug *d3dDebug;
	HRESULT hr = S_OK;

	if (g_pd3dDevice11 != nullptr)
	{
		hr = g_pd3dDevice11->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		}

		ReleaseCOM(d3dDebug);
		ReleaseCOM(g_pd3dDevice11);
	}

#endif
}

void IRoot::Mainloop()
{
	MSG msg;//��Ϣ��
	ZeroMemory(&msg, sizeof(msg));

	//�ڳ���û�н��յ����˳�����Ϣ��ʱ��
	while (msg.message != WM_QUIT)
	{
		/*PM_REMOVE  PeekMessage�������Ϣ�Ӷ����������
		������PeekMesssge���Բ鿴�ķ�ʽ��ϵͳ�л�ȡ��Ϣ��
		���Բ�����Ϣ��ϵͳ���Ƴ����Ƿ�����������
		��ϵͳ����Ϣʱ������FALSE������ִ�к������롣*/

		//����Ϣ��ʱ��Ͻ������� ��Peek����TRUE��ʱ��
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);//������Ϣ�����͵�windows��Ϣ����
			DispatchMessage(&msg);//������Ϣ
		}
		else
			//��Ϣ����û���������� �Ǿ͸���ѭ��
		{
			switch (mMainLoopStatus)
			{
			case NOISE_MAINLOOP_STATUS_BUSY:
				//�����ѭ��������Ч
				if (m_pMainLoopFunction)
				{
					(*m_pMainLoopFunction)();
				}
					break;

			case NOISE_MAINLOOP_STATUS_QUIT_LOOP:
				return;
				break;

			default:
				break;

			}//switch
		}
	};

	//ReleaseAll();
	UnregisterClass(mRenderWindowClassName, mRenderWindowHINSTANCE);
}

void IRoot::SetMainLoopFunction( void (*pFunction)(void) )
{
	m_pMainLoopFunction =pFunction;
}

void IRoot::SetMainLoopStatus(NOISE_MAINLOOP_STATUS loopStatus)
{
	mMainLoopStatus = loopStatus;
}


/************************************************************************
										 PRIVATE                               
************************************************************************/

//windows message handle
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*HDC			hdc ;
	PAINTSTRUCT ps ;
	RECT        rect ;*/
	switch (message)
	{
	case WM_CREATE:
		return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}

bool IRoot::mFunction_InitWindowClass(WNDCLASS* wc)
{
	wc->style = CS_HREDRAW | CS_VREDRAW; //��ʽ
	wc->cbClsExtra = 0;
	wc->cbWndExtra = 0;
	wc->hInstance = mRenderWindowHINSTANCE;//����ʵ��������windows�Զ��ַ�
	wc->hIcon = LoadIcon(NULL, IDI_APPLICATION);//��ʾ�����ͼ��titlte
	wc->hCursor = LoadCursor(NULL, IDC_ARROW);//���ڹ��
	wc->hbrBackground = NULL;// (HBRUSH)GetStockObject(WHITE_BRUSH);//����ˢ
	wc->lpszMenuName = NULL;
	wc->lpfnWndProc = WndProc;//���ô������windws��Ϣ����
	wc->lpszClassName = mRenderWindowClassName;//��������

	if (!RegisterClass(wc))//ע�ᴰ����
	{
		ERROR_MSG("IRoot: Create Window failed! Windows NT (or newer) required");
		return false;
	}
	else
	{
		return true;
	};

};

HWND IRoot::mFunction_InitWindow(UINT windowWidth,UINT windowHeight)
{
	UINT scrWidth = GetSystemMetrics(SM_CXSCREEN);
	UINT scrHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd;
	hwnd = CreateWindow(
		mRenderWindowClassName,      // window class name
		mRenderWindowTitle,   // window caption
		WS_OVERLAPPEDWINDOW, // window style ------WS_OVERLAPPEDWINDOW/WS_POPUP
		//WS_POPUP, // window style ------WS_OVERLAPPEDWINDOW/WS_POPUP
		scrWidth/6,// initial x position ---------CW_USEDEFAULT
		scrHeight/6,// initial y position
		windowWidth,// initial x size
		windowHeight,// initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		mRenderWindowHINSTANCE, // program instance handle
		NULL);

	if (hwnd == 0)
	{
		//����ʧ��
		return 0;
	}
	else
	{
		//�����ɹ�
		ShowWindow(hwnd, SW_RESTORE);//��ʾ����
		UpdateWindow(hwnd);//���´���
		return hwnd;
	};

};

bool	IRoot::mFunction_CreateEffectFromMemory()
{
	std::vector<char> compiledShader;
	HRESULT hr = S_OK;

	//Load FXO file
#ifdef _DEBUG
	if (!IFileIO::ImportFile_PURE("shader//Main_d.fxo", compiledShader))
	{
		ERROR_MSG("Root : critical error! Compiled shader (Debug mode) not found!!");
		return false;
	}
#else
	if (!IFileIO::ImportFile_PURE("shader//Main.fxo", compiledShader))
	{
		ERROR_MSG("Root : critical error! Compiled shader(Release mode) not found!!");
		return false;
	}
#endif

	//Create Effect Framework
	hr = D3DX11CreateEffectFromMemory(&compiledShader.at(0), compiledShader.size(), 0, g_pd3dDevice11, &g_pFX);
//#else

	//N_InternalResourceInfo shaderInResource = Noise3D::GetInternalResource(NOISE_INTERNAL_RESOURCE_MAINSHADER);
	//Create Effect Framework
	//hr = D3DX11CreateEffectFromMemory(shaderInResource.pBuff, shaderInResource.size(), 0, g_pd3dDevice11, &g_pFX);

//#endif


	HR_DEBUG(hr, "IRoot : load compiled shader failed");

	return true;
}


