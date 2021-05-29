#include "Application.h"
#include "Figure.h"
//====================================================================================================
//												INFO_ZONE
//====================================================================================================
HDC memDC;
HDC mainDC;

HBITMAP oldBitmap;
HBITMAP bitmap;

triangular_pyramid pyramid;
triangular_pyramid pyramid_1;

POINT posCursor;
POINT tmpDT;

char direct = NULL;
char flCursor = NULL;
char flDraw = FALSE;
//====================================================================================================
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//====================================================================================================
//												 OBJECTS
//====================================================================================================
Canvas paint_canvas;
Canvas top_menu;
Canvas top_direct_menu;

Button move_btn;
Button rotate_btn;
Button scale_btn;
Button create_btn;

Button x_btn;
Button y_btn;
Button z_btn;
//====================================================================================================
//												 CANVAS
//====================================================================================================
const wchar_t NAME_CANVAS[] = L"Canvas";
//====================================================================================================
int Canvas::fillData(HINSTANCE hInstance) {
	WNDCLASS canvas = {};

	canvas.lpszClassName = NAME_CANVAS;
	canvas.lpfnWndProc = WndProc;
	canvas.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	RegisterClass(&canvas);

	return NULL;
}

void Canvas::createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos, int Height, int Width) {
	this->xPos = xPos;
	this->yPos = yPos;

	if (Height != NULL && Width != NULL) {
		this->Height = Height;
		this->Width = Width;
	}

	wchar_t m_report[256];

	if ((this->hWnd = CreateWindow(
		NAME_CANVAS,
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,

		xPos, yPos, this->Width, this->Height,

		parent,
		NULL,
		hInstance,
		NULL
	)) == NULL){
		MessageBox(parent, L"Error in canvas", L"Error", MB_OK);
		PostQuitMessage(EXIT_FAILURE);
	}
}
//====================================================================================================
//====================================================================================================
//												BUTTON
//====================================================================================================
//====================================================================================================
const wchar_t NAME_BUTTON[] = L"BUTTON";
//====================================================================================================
//====================================================================================================
void Button::createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos, 
					   int Height, int Width , const wchar_t NAME[], int hMenu, int BITMAP_ID) {
	HBITMAP hBM;
	
	this->xPos = xPos;
	this->yPos = yPos;
	this->Height = Height;
	this->Width = Width;

	wchar_t m_report[256];

	if ((hWnd = CreateWindowExW(
		NULL,
		NAME_BUTTON,
		NAME,
		WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,

		xPos, yPos, Width, Height,

		parent,
		(HMENU) hMenu,
		hInstance,
		NULL
	)) == NULL) {
		MessageBox(parent, L"Error in canvas", L"Error", MB_OK);
		PostQuitMessage(EXIT_FAILURE);
	}
}
//====================================================================================================
//====================================================================================================
//											APPLICATION
//====================================================================================================
//====================================================================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	return appMainWnd.classWndProc(hwnd, uMsg, wParam, lParam);
}

int App::fillData(HINSTANCE &hInstance, const wchar_t clsName[], int nCmdShow) {
	this->clsName = clsName;
	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;

	wcMain.hInstance = hInstance;
	wcMain.lpfnWndProc = WndProc;
	wcMain.lpszClassName = clsName;
	// wcMain.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(MAIN_ICON), IMAGE_ICON, 32, 32, 0);
	wcMain.style = CS_HREDRAW | CS_VREDRAW;
	wcMain.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);

	return NULL;
}

int App::Init() {
	// register main wnd
	RegisterClassW(&wcMain);

	// craete window
	hMainWnd = CreateWindow(
		wcMain.lpszClassName,
		wcMain.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	// window update
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	paint_canvas.fillData(hInstance);
	// create new windows
	paint_canvas.createWnd(hMainWnd, hInstance, 0, 0, 500, 500);
	top_menu.createWnd(hMainWnd, hInstance, 0, 0 , 60, 208);
	top_direct_menu.createWnd(hMainWnd, hInstance, 210, 0, 32, 148);

	HWND top_tmp = top_menu.getHWND();
	// create buttons
	move_btn.createWnd( top_tmp, hInstance, 2, 2, 26, 100, L"MOVE", 10, NULL);
	scale_btn.createWnd( top_tmp, hInstance, 104, 2, 26, 100, L"SCALE", 20, NULL);
	rotate_btn.createWnd( top_tmp, hInstance, 2, 30, 26, 100, L"ROTATE", 30, NULL);
	create_btn.createWnd( top_tmp, hInstance, 104, 30, 26, 100, L"CREATE", 40, NULL);

	top_tmp = top_direct_menu.getHWND();

	x_btn.createWnd( top_tmp, hInstance, 2, 2, 26, 46, L"X", 1, NULL);
	y_btn.createWnd( top_tmp, hInstance, 50, 2, 26, 46, L"Y", 2, NULL);
	z_btn.createWnd( top_tmp, hInstance, 98, 2, 26, 46, L"Z", 3, NULL);

	SendMessageW(hMainWnd, WM_CREATE, NULL, NULL);

	return NULL;
}
//====================================================================================================
//====================================================================================================
int App::Run() {
	MSG uMsg = {};
	LPARAM lParam;
	RECT rw;

	// reaupdate function
	GetClientRect(hMainWnd, &rw);

	WORD nWidth = rw.right - rw.left; // LOWORD(lParam);   width of client area 
	WORD nHeight = rw.bottom - rw.top; //HIWORD(lParam);  height of client area
	lParam = nWidth | (nHeight << 16);

	// update window
	updateWindow(lParam);

	// start recieving messages
	while (GetMessageW( &uMsg, hMainWnd, NULL, NULL) > 0) {
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}

	return NULL;
}
//====================================================================================================
// update position of window
void updateWindow(LPARAM lParam) {
	/* positions */
	int nWidth;
	int nHeight;

	int centerWidth;
	int centerHeight;
	/* positions */

	// error code
	int Error;
	// error report
	wchar_t errReport[256];

	// get new size
	nWidth = LOWORD(lParam);  // width of client area
	nHeight = HIWORD(lParam); // height of client area 

	// calc center
	centerWidth = (nWidth - paint_canvas.getWidth()) / 2;
	centerHeight = (nHeight - paint_canvas.getHeight()) / 2;

	// move paintlist
	if (paint_canvas.getHWND() != NULL) {
		if (centerHeight < 70)
			centerHeight = 70;
		if (centerWidth < 40)
			centerWidth = 40;

		if (centerHeight == 70 && centerWidth == 40)
			return;

		if (!MoveWindow(paint_canvas.getHWND(), centerWidth, centerHeight, paint_canvas.getWidth(), paint_canvas.getHeight(), true)) {
			// get error code
			Error = GetLastError();

			// convert to wchar_t
			swprintf_s(errReport, L"%d", Error);
			MessageBox(appMainWnd.getHWND(), errReport, L"Error", MB_OK);
		}
	}

	return;
}
//====================================================================================================
LRESULT CALLBACK App::classWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RECT rc_paint_cnv = {};
	PAINTSTRUCT ps;
	HWND tmp_hwnd;
	HDC tmp_dc;

	int res;

	if (hwnd == paint_canvas.getHWND())
		transform_WndProc(hwnd, uMsg, wParam, lParam);

	switch (uMsg){
	case WM_CREATE:
		// create memory buffer
		// and fill them with white
		tmp_hwnd = paint_canvas.getHWND();

		if ( tmp_hwnd != NULL && memDC == NULL) {
			mainDC = GetDC(paint_canvas.getHWND());

			GetClientRect(paint_canvas.getHWND(), &rc_paint_cnv);

			memDC = CreateCompatibleDC(mainDC);
			bitmap = CreateCompatibleBitmap(mainDC, paint_canvas.getWidth(), paint_canvas.getHeight());
			oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

			FillRect(memDC, &rc_paint_cnv, WHITE_BRUSH);

			ReleaseDC(paint_canvas.getHWND(), mainDC);
		}
		break;
	case WM_COMMAND:
		res = LOWORD(wParam);

		switch (res)
		{
		// X direct
		case 1:
			direct = X;
			break;
		// Y direct
		case 2:
			direct = Y;
			break;
		// Z direct
		case 3:
			direct = Z;
			break;
		// MOVE
		case 10:
			flCursor = MOVE;
			break;
		// SCALE
		case 20:
			flCursor = SCALE;
			break;
		// ROTATE
		case 30:
			flCursor = ROTATE;
			break;
		// CREATE
		case 40:
			pyramid.displayPyramid(memDC, RGB(0, 0, 0));
			InvalidateRect(paint_canvas.getHWND(), NULL, FALSE);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		// copy mem to main frame
		// start paint
		tmp_dc = BeginPaint( paint_canvas.getHWND(), &ps);

		GetClientRect(paint_canvas.getHWND(), &rc_paint_cnv);

		BitBlt(tmp_dc, rc_paint_cnv.left, rc_paint_cnv.top, rc_paint_cnv.right - rc_paint_cnv.left, 
			   rc_paint_cnv.bottom - rc_paint_cnv.top, memDC,
			   rc_paint_cnv.left, rc_paint_cnv.top, SRCCOPY);

		EndPaint( paint_canvas.getHWND(), &ps);
		break;
	case WM_SIZE:
		// change positions windows
		updateWindow(lParam);

		// check!!! maybe you can delete that without damage
		// send message WM_PAINT
		InvalidateRect(paint_canvas.getHWND(), NULL, FALSE);
		break;
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		SelectObject(memDC, oldBitmap);
		DeleteDC(memDC);

		PostQuitMessage(EXIT_SUCCESS);
		break;
	default:
		break;
	}

	// default reaction
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//====================================================================================================
void transform_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		// get cursor pos
		posCursor.x = LOWORD(lParam);
		posCursor.y = HIWORD(lParam);

		if (flCursor == MOVE || flCursor == SCALE || flCursor == ROTATE) {
			flDraw = true;

			// save new coord
			tmpDT.x = posCursor.x;
			tmpDT.y = posCursor.y;
		}

		break;
	case WM_MOUSEMOVE:
		// transform line
		if (((flCursor & MOVE) || (flCursor & SCALE) || (flCursor & ROTATE)) && flDraw == true) {
			// calc new position
			pyramid.calcNewPos(lParam, flCursor, direct, memDC, tmpDT);

			// save new coord
			tmpDT.x = LOWORD(lParam);
			tmpDT.y = HIWORD(lParam);
		}

		InvalidateRect(paint_canvas.getHWND(), NULL, FALSE);
		break;
	case WM_LBUTTONUP:
		// stop drawing
		if (flDraw == true)
			flDraw = false;
		break;
	}

	return;
}