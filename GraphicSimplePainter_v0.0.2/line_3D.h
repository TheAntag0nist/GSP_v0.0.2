#pragma once
#include "GSP_core.h"
#include "Line.h"
//=============================================================================
//								MATRICES FOR 3D
//=============================================================================
static Matrix<float> proection(4, 4);

static Matrix<float> transform_3D(4, 4);
static Matrix<float> posPnt_3D_1(4, 1);
static Matrix<float> posPnt_3D_2(4, 1);
static Matrix<float> resPnt_1_3D(4, 1);
static Matrix<float> resPnt_2_3D(4, 1);
//==============================================================================
//								3D POINT STRUCT
//==============================================================================
typedef struct POINT_3D {
	float x;
	float y;
	float z;

	POINT_3D(int x_data = 0, int y_data = 0, int z_data = 0) {
		x = x_data;
		y = y_data;
		z = z_data;
	}
} pnt;
//==============================================================================
//								CLASS LINE_3D
//==============================================================================
class Line_3D {
private:
	pnt startPoint;
	pnt endPoint;
public:
	// basic constructor
	Line_3D() {
		startPoint = { 0, 0, 0};
		endPoint = startPoint;
	}

	void setPnts(pnt str, pnt end) {
		startPoint = str;
		endPoint = end;
	}
	pnt getPnts(int i) {
		if (i == 0)
			return startPoint;
		else if (i == 1)
			return endPoint;
	}

	void displayLine_3D(HDC memDC, COLORREF color, pnt center = {0,0,0});

	// transform point
	void transformPoint(LPARAM lParam, char flCursor, char flVect, POINT tmpDT, pnt center);

	// convert to 2d point
	POINT* convTo2D(pnt center);
};