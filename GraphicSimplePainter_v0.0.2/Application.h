#pragma once
#include "GSP_core.h"
//====================================================================================================
class Window {
protected:
	HWND hWnd;
	HDC hWndDC;

	int xPos;
	int yPos;
	int Height;
	int Width;
public:
	void setHeight(int height) {
		this->Height = height;
	}
	void setWidth(int width) {
		this->Width = width;
	}
	void setSize(int width, int height) {
		this->Width = width;
		this->Height = height;
	}

	int getHeight() {
		return this->Height;
	}
	int getWidth() {
		return this->Width;
	}

	HWND getHWND() {
		return hWnd;
	}
};
//====================================================================================================
class Canvas: public Window {
public:
	static int fillData(HINSTANCE hInstance);

	void createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos, int Height = 0x00, int Width = 0x00);
};
//====================================================================================================
class Button:public Window {
public:
	void createWnd(HWND parent, HINSTANCE hInstance, int xPos, int yPos,
				   int Height, int Width, const wchar_t NAME[], int hMenu, int BITMAP_ID);
};
//====================================================================================================
class App {
private:
	HWND hMainWnd;
	WNDCLASSW wcMain;
	HINSTANCE hInstance;
	LPCWSTR clsName;

	HDC hMnWndDC;

	int nCmdShow;

	int width;
	int height;

	void reUpFunc(LPARAM lParam);
public:
	// default constructor
	App() {};

	HWND getHWND() {
		return hMainWnd;
	}

	// add data about WNDCLASS
	int fillData(HINSTANCE &hInstance, const wchar_t clsName[], int nCmdShow = SW_NORMAL);
	int Init();

	// get messages and run classWndProc->WndProc 
	int Run();

	// realization of WndProc in class
	LRESULT CALLBACK classWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
//====================================================================================================
static App appMainWnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//====================================================================================================
void updateWindow(LPARAM lParam);
void transform_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);