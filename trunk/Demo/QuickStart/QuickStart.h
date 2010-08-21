#ifndef _QuickStart_H_
#define _QuickStart_H_

#include <windows.h>
#include <string>
#include <map>
#include <gdiplus.h>

#pragma comment( lib, "gdiplus.lib" ) 

namespace RCP
{
	class Renderer;
}
//»æÖÆ·½·¨
class GDIPainting;

class QSApplication
{
public:
	typedef std::map<std::string, std::string> CustomParameter;

	struct AppParam
	{
		std::string name;
		unsigned int left;
		unsigned int top;
		unsigned int width;
		unsigned int height;
		bool fullScreen;

		AppParam():
			name("hello world!"),
			left(100),top(100),width(640),height(480),
			fullScreen(false)
		{}
	};
public :
	static QSApplication* mSingleton;

	static  QSApplication* getSingletonPtr()
	{
		return mSingleton;
	}
public:
	QSApplication();
	virtual ~QSApplication();
	void start(const AppParam* param = NULL);

	const AppParam& getParam()
	{
		return mAppParam;
	}

protected:
	virtual void init(RCP::Renderer& renderer, const AppParam& param) = 0;
	virtual void reset(RCP::Renderer& renderer, const AppParam& param) = 0;
	virtual void destroy(RCP::Renderer& renderer, const AppParam& param) = 0;
	virtual void renderOneFrame(RCP::Renderer& renderer, const AppParam& param) = 0;
private:
	void begin();
	void createWindow();
	void initGDI();
	void initRenderer();
	void mainLoop();
	void shutdown();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND	mHWnd;					// Win32 Window handle
	ULONG_PTR mGdiplusToken;
	AppParam mAppParam;
	bool mRunning;
	RCP::Renderer* mRenderer;
	GDIPainting* mGdiPainting;


};

#endif //_QuickStart_H_