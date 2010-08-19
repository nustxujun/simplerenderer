#include "QuickStart.h"
#include "RendererHeader.h" 
#include "GDIPainting.h"
using namespace RCP;

QSApplication* QSApplication::mSingleton = NULL;

QSApplication::QSApplication():
	mRunning(false)
{
	assert(mSingleton == 0)	;
	mSingleton = this;
}

QSApplication::~QSApplication()
{}

void QSApplication::start()
{
	begin();
	mRunning = true;
	mainLoop();
	mRunning = false;
	shutdown();
}

void QSApplication::begin()
{
	createWindow();
	initGDI();
	initRenderer();


	init(*mRenderer,mAppParam);
}

void QSApplication::initRenderer()
{
	RendererParameters rp;
	rp.backBufferWidth = mAppParam.width;
	rp.backBufferHeight = mAppParam.height;
	rp.backBufferPixelFormat = PF_A8R8G8B8;
	mRenderer = new Renderer();
	mRenderer->initialize(rp);
	mGdiPainting = new GDIPainting();
	mGdiPainting->init(GetDC(mHWnd),rp.backBufferWidth,rp.backBufferHeight,rp.backBufferPixelFormat);
	mRenderer->setPaintingMethod(mGdiPainting);
	

}

void QSApplication::createWindow()
{

	DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN;
	DWORD dwStyleEx = 0;

	int outerw, outerh;
	//if (mIsFullScreen)
	//{
	//	dwStyle |= WS_POPUP;
	//	dwStyleEx |= WS_EX_TOPMOST;
	//	outerw = mAppParam.width;
	//	outerh = mAppParam.height;
	//	mAppParam.left = mAppParam.top = 0;
	//}
	//else
	{
		{
			//if (border == "none")
				//dwStyle |= WS_POPUP;
			//else if (border == "fixed")
				dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION |
				WS_SYSMENU | WS_MINIMIZEBOX;
			//else
			//	dwStyle |= WS_OVERLAPPEDWINDOW;
		}

		int screenw = GetSystemMetrics(SM_CXSCREEN);
		int screenh = GetSystemMetrics(SM_CYSCREEN);

		//if (!outerSize)
		{
			// calculate overall dimensions for requested client area
			RECT rc = { 0, 0, mAppParam.width, mAppParam.height };
			AdjustWindowRect(&rc, dwStyle, false);

			// clamp window dimensions to screen size
			outerw = (rc.right-rc.left < screenw)? rc.right-rc.left : screenw;
			outerh = (rc.bottom-rc.top < screenh)? rc.bottom-rc.top : screenh;
		}

		// center window if given negative coordinates
		if (mAppParam.left < 0)
			mAppParam.left = (screenw - outerw) / 2;
		if (mAppParam.top < 0)
			mAppParam.top = (screenh - outerh) / 2;

		// keep window contained in visible screen area
		if (mAppParam.left > screenw - outerw)
			mAppParam.left = screenw - outerw;
		if (mAppParam.top > screenh - outerh)
			mAppParam.top = screenh - outerh;

	}



	WNDCLASS wc = 
	{
		CS_OWNDC, 
		QSApplication::WndProc, 
		0, 0, 
		GetModuleHandle(NULL),
		LoadIcon(NULL, IDI_APPLICATION), 
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH), 
		NULL, 
		mAppParam.name.c_str()
	};

	RegisterClass(&wc);

	mHWnd = CreateWindowEx(dwStyleEx, mAppParam.name.c_str(), mAppParam.name.c_str(),
		dwStyle, mAppParam.left, mAppParam.top, outerw, outerh, NULL, 0, GetModuleHandle(NULL), this);

	RECT rc;
	// top and left represent outer window position
	GetWindowRect(mHWnd, &rc);
	mAppParam.top = rc.top;
	mAppParam.left = rc.left;
	// width and height represent drawable area only
	GetClientRect(mHWnd, &rc);
	mAppParam.width = rc.right;
	mAppParam.height = rc.bottom;


	 ShowWindow(mHWnd, SW_SHOW);
	 UpdateWindow(mHWnd);
	
}

void QSApplication::initGDI()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&mGdiplusToken, &gdiplusStartupInput, NULL);
}


void QSApplication::mainLoop()
{
	MSG msg;
	RECT rect;
	ZeroMemory( &msg, sizeof(msg) );
	while (msg.message!=WM_QUIT && mRunning/*GetMessage(&msg, NULL, 0, 0)*/)
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			//render();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			renderOneFrame(*mRenderer,mAppParam);
			
			rect.bottom = mAppParam.height;
			rect.left = 0;
			rect.right = mAppParam.width;
			rect.top = 0;
			InvalidateRect(mHWnd,&rect,false);
		}
	}
}

void QSApplication::shutdown()
{
	destroy(*mRenderer,mAppParam);
	mRenderer->release();
	delete mRenderer;
	delete mGdiPainting;
	Gdiplus::GdiplusShutdown(mGdiplusToken);
}

LRESULT CALLBACK QSApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	QSApplication* app =  QSApplication::getSingletonPtr();
	switch (message)
	{
	case WM_PAINT:
	
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		if (app->mRunning && app->mRenderer->isInitialized())
		{
			app->mRenderer->renderNow();
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
