#pragma once
#include "GSP_core.h"
#include "line_3D.h"
#include "Line.h"
#include "Matrix.h"

static Matrix<float> plane(3, 3);
//=============================================================================
//							BASIC COORDINATES for OBJECT
//=============================================================================
static float points_matrix[4][3] = {
	{ 150.0, 140.0, 200.5},
	{ 200.0, 140.0, 200.5},
	{ 175.0, 183.5, 200.5},
	{ 175.0, 167.0, 220.0}
};
//=============================================================================
class triangular_pyramid {
private:
	pnt points[4];

	Line_3D lines[6];
public:
	triangular_pyramid() {
		for (int i = 0; i < 4; ++i) {
			points[i].x = points_matrix[i][0];
			points[i].y = points_matrix[i][1];
			points[i].z = points_matrix[i][2];
		}

		// top point lines
		lines[0].setPnts(points[0], points[1]);
		lines[1].setPnts(points[0], points[2]);
		lines[2].setPnts(points[0], points[3]);

		// basic lines
		lines[3].setPnts(points[1], points[2]);
		lines[4].setPnts(points[2], points[3]);
		lines[5].setPnts(points[3], points[1]);
	}

	void calcNewPos(LPARAM lParam, char flCursor, char flVect, HDC memDC, POINT tmpDT);

	// display pyramid
	void displayPyramid(HDC memDC, COLORREF color, pnt center = {0,0,0});
};