#include "GSP_core.h"

HDC dc;
HBITMAP bitmap_data;
HBITMAP oldHBITMAP;

int width;
int height;

void init_graph(int x_size, int y_size) {
	width = x_size;
	height = y_size;
}

DWORD* get_dib(HDC memDC) {
	dc = CreateCompatibleDC(memDC);

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof(BITMAPINFOHEADER));

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = 398;
	bmi.bmiHeader.biHeight = 398;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount = 32;

	DWORD* bitmapBits;
	bitmap_data = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&bitmapBits, 0, 0);
	oldHBITMAP = (HBITMAP)SelectObject(dc, bitmap_data);

	BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

	return bitmapBits;
}

void delete_DC() {
	SelectObject(dc, oldHBITMAP);
	DeleteObject(bitmap_data);
	DeleteDC(dc);
}

int set_pixel(DWORD* pixel_mass, int x, int y, COLORREF color) {
	pixel_mass[x + y * width] = color;

	return 0;
}

DWORD get_pixel(DWORD* pixel_mass, int x, int y) {
	return pixel_mass[x + y * width];
}