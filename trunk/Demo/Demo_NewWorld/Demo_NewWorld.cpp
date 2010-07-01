// Demo_NewWorld.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo_NewWorld.h"
#include <comdef.h>
#include <gdiplus.h>
#include "GDIPainting.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
ULONG_PTR m_gdiplusToken;
HWND g_hw;

using namespace RCP;
GDIPainting g_Painting;
Renderer g_Renderer;
VertexBuffer* g_vb;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



void render()
{

	if (!g_Renderer.isInitialized())
		return ;
	// TODO: Add any drawing code here...
	if (g_Renderer.isInitialized())
	{
		Matrix4X4 world;
		static float i = 0;
		i += 0.1;
		float r  = sin(i);
		world = world * r;
		world.m[3][3] = 1;
		g_Renderer.setMatrix(RCP::TS_WORLD,world);
		
		g_Renderer.setVertexBuffer(g_vb);
		g_Renderer.draw(PT_TRIANGLESTRIP,0,2);
		g_Renderer.renderNow();
	}
;


}	

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO_NEWWORLD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO_NEWWORLD));

	// Main message loop:
	 ZeroMemory( &msg, sizeof(msg) );
	 bool bGotMsg;
	while (msg.message!=WM_QUIT/*GetMessage(&msg, NULL, 0, 0)*/)
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
			RECT rect;
			rect.bottom = 512;
			rect.left = 0;
			rect.right = 512;
			rect.top = 0;
			InvalidateRect(g_hw,&rect,false);
		}


		//bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

		//if( bGotMsg )
		//{
		//	// Translate and dispatch the message
		//	if( hAccelTable == NULL  || 
		//		0 == TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		//	{
		//		TranslateMessage( &msg );
		//		DispatchMessage( &msg );
		//	}
		//	else
		//	UpdateWindow(g_hw);
		//
		//}


	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);


	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO_NEWWORLD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DEMO_NEWWORLD);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

void prepare()
{
	Matrix4X4 view, projection,world;
	MatrixUtil::getViewSpace(view,Vector3(0,0,-3),Vector3(0,0,0),Vector3(0,1,0));
	g_Renderer.setMatrix(TS_VIEW,view);	

	MatrixUtil::getPerspectiveProjectionSpace(projection,3.14159265 / 4,64/(float)64,0.1,10);
	g_Renderer.setMatrix(TS_PROJECTION,projection);	


	g_Renderer.setMatrix(TS_WORLD,world);

	VertexDeclaration vd2;
	vd2.addElement(VET_FLOAT3,VES_POSITION);
	vd2.addElement(VET_COLOUR,VES_DIFFUSE);
	vd2.addElement(VET_FLOAT2,VES_TEXTURE_COORDINATES);

	struct vertexFormat
	{
		float x,y,z;
		int color;
		float u,v;
	};
	vertexFormat vertexData[] = 
	{
		-1.0f,-1.0f,3.0f,0xffff0000,1.0f,1.0f,
		-1.0f,1.0f,3.0f,0xff00ff00,1.0f,0.0f,
		1.0f,-1.0f,0.f,0xff0000ff,0.0f,1.0f,

		1.0f,-1.0f,0.f,0xffff00ff,0.0f,1.0f,
		-1.0f,1.0f,3.0f,0xff00ff00,1.0f,0.0f,
		1.0f,1.0f,0.0f,0xff0000ff,0.0f,0.0f,
	};

	g_vb = g_Renderer.createVertexBuffer(6,vd2);
	g_vb->fill(0,6,vertexData);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hw = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   Gdiplus::GdiplusStartupInput gdiplusStartupInput;
   Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

   RendererParameters rp;
   rp.backBufferWidth = 300;
   rp.backBufferHeight = 300;
   rp.backBufferPixelFormat = PF_A8R8G8B8;
   g_Renderer.initialize(rp);
   g_Renderer.setPaintingMethod(&g_Painting);
   prepare();

   return TRUE;
}

	

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		if (g_Renderer.isInitialized())
		{
			g_Painting.prepare(hdc);
			render();
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
