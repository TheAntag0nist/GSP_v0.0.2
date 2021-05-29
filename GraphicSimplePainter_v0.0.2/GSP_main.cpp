#include "Application.h"
//====================================================================================================
//====================================================================================================
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	const wchar_t wndName[] = L"GraphicSimplePainter - v0.0.2";

	// set basic info about window
	appMainWnd.fillData(hInstance, wndName, SW_SHOWMAXIMIZED);

	// initializing
	appMainWnd.Init();
	// run program
	appMainWnd.Run();

	return NULL;
}