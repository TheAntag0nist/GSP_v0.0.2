#pragma once
#include <windows.h>
#include <cmath>
#include <stdio.h>
#include "Matrix.h"
//==============================================================================
//									VIEW SETTINGS
//==============================================================================
#define Z_NEAR 200.0
#define Z_FAR 200

#define LEFT -0.5
#define RIGHT 0.5

#define TOP 0.5
#define BOTTOM -0.5

#define FOCUS 500
//==============================================================================
//									2D constants
//==============================================================================
#define MOVE 0x03
#define ROTATE 0x04
#define SCALE 0x05
//==============================================================================
//									3D constants
//==============================================================================
#define X 0x50
#define Y 0x51
#define Z 0x52

#define MOVE_X 0x30
#define MOVE_Y 0x31
#define MOVE_Z 0x32

#define ROTATE_X 0x40
#define ROTATE_Y 0x041
#define ROTATE_Z 0x042

#define WIDTH 500
#define HEIGHT 500
//==============================================================================
static Matrix<float> world(4, 4);
//==============================================================================
//									DIB SECTIONS API
//==============================================================================
void init_graph(int x_size, int y_size);

DWORD* get_dib(HDC memDC);
void delete_DC();

int set_pixel(DWORD* pixel_mass, int x, int y, COLORREF color = 0x00);
DWORD get_pixel(DWORD* pixel_mass, int x, int y);