#pragma once
#include "line_3D.h"
//=============================================================================
void Line_3D::transformPoint(LPARAM lParam, char flCursor, char flVect, POINT tmpDT, pnt center) {
	float scaleCoeff = static_cast<float>(110) / 100;
	
	float sinAngle = 0.0174524064;
	float cosAngle = 0.999847695;
	
	POINT deltaPos;
	pnt new_coord;

	// delta from start
	deltaPos.x = LOWORD(lParam) - tmpDT.x;
	deltaPos.y = HIWORD(lParam) - tmpDT.y;

	resPnt_1_3D.zeroM();
	resPnt_2_3D.zeroM();

	// save start point
	posPnt_3D_1.matrix[0][0] = startPoint.x;
	posPnt_3D_1.matrix[1][0] = startPoint.y;
	posPnt_3D_1.matrix[2][0] = startPoint.z;
	posPnt_3D_1.matrix[3][0] = 1;

	// save end point
	posPnt_3D_2.matrix[0][0] = endPoint.x;
	posPnt_3D_2.matrix[1][0] = endPoint.y;
	posPnt_3D_2.matrix[2][0] = endPoint.z;
	posPnt_3D_2.matrix[3][0] = 1;

	// fill matrix as 1 matrix
	transform_3D.oneM();

	// rewrite as not Z
	// flVect != Z
	if (flCursor == MOVE) {
		if (flVect != Z) {
			transform_3D.matrix[0][3] = deltaPos.x;
			transform_3D.matrix[1][3] = deltaPos.y;
		}else {
			if(deltaPos.x > 0)
				transform_3D.matrix[2][3] = 5.25;
			else 
				transform_3D.matrix[2][3] = -5.25;
		}
	
		transform_3D.multi(posPnt_3D_1, resPnt_1_3D);
		transform_3D.multi(posPnt_3D_2, resPnt_2_3D);
	}
	else if (flCursor == SCALE) {
		// save start point
		posPnt_3D_1.matrix[0][0] -= center.x;
		posPnt_3D_1.matrix[1][0] -= center.y;
		posPnt_3D_1.matrix[2][0] -= center.z;
		posPnt_3D_1.matrix[3][0] = 1;

		// save end point
		posPnt_3D_2.matrix[0][0] -= center.x;
		posPnt_3D_2.matrix[1][0] -= center.y;
		posPnt_3D_2.matrix[2][0] -= center.z;
		posPnt_3D_2.matrix[3][0] -= 1;

		if (deltaPos.x > 0) {
			transform_3D.matrix[0][0] = scaleCoeff;
			transform_3D.matrix[1][1] = scaleCoeff;
			transform_3D.matrix[2][2] = scaleCoeff;
		}else {
			transform_3D.matrix[0][0] = 1.0 / scaleCoeff;
			transform_3D.matrix[1][1] = 1.0 / scaleCoeff;
			transform_3D.matrix[2][2] = 1.0 / scaleCoeff;
		}

		transform_3D.multi(posPnt_3D_1, resPnt_1_3D);
		transform_3D.multi(posPnt_3D_2, resPnt_2_3D);
	}
	else if (flCursor == ROTATE) {
		// save start point
		posPnt_3D_1.matrix[0][0] -= center.x;
		posPnt_3D_1.matrix[1][0] -= center.y;
		posPnt_3D_1.matrix[2][0] -= center.z;
		posPnt_3D_1.matrix[3][0] = 1;

		// save end point
		posPnt_3D_2.matrix[0][0] -= center.x;
		posPnt_3D_2.matrix[1][0] -= center.y;
		posPnt_3D_2.matrix[2][0] -= center.z;
		posPnt_3D_2.matrix[3][0] -= 1;

		if (flVect != Y && flVect != Z) {
			transform_3D.matrix[1][1] = cosAngle;
			transform_3D.matrix[2][1] = -sinAngle;
			transform_3D.matrix[1][2] = sinAngle;
			transform_3D.matrix[2][2] = cosAngle;
		}
		else if (flVect == Y) {
			transform_3D.matrix[0][0] = cosAngle;
			transform_3D.matrix[0][2] = -sinAngle;
			transform_3D.matrix[2][0] = sinAngle;
			transform_3D.matrix[2][2] = cosAngle;
		}
		else if (flVect == Z) {
			transform_3D.matrix[0][0] = cosAngle;
			transform_3D.matrix[0][1] = sinAngle;
			transform_3D.matrix[1][0] = -sinAngle;
			transform_3D.matrix[1][1] = cosAngle;
		}

		transform_3D.multi(posPnt_3D_1, resPnt_1_3D);
		transform_3D.multi(posPnt_3D_2, resPnt_2_3D);
	}

	if (flCursor == SCALE || flCursor == ROTATE) {
		resPnt_1_3D.matrix[0][0] += center.x;
		resPnt_1_3D.matrix[1][0] += center.y;
		resPnt_1_3D.matrix[2][0] += center.z;

		resPnt_2_3D.matrix[0][0] += center.x;
		resPnt_2_3D.matrix[1][0] += center.y;
		resPnt_2_3D.matrix[2][0] += center.z;
	}

	startPoint.x = resPnt_1_3D.matrix[0][0];
	startPoint.y = resPnt_1_3D.matrix[1][0];
	startPoint.z = resPnt_1_3D.matrix[2][0];

	endPoint.x = resPnt_2_3D.matrix[0][0];
	endPoint.y = resPnt_2_3D.matrix[1][0];
	endPoint.z = resPnt_2_3D.matrix[2][0];
}
//=============================================================================
POINT* Line_3D::convTo2D(pnt center) {
	POINT* point = new POINT[2];

	posPnt_3D_1.zeroM();
	resPnt_1_3D.zeroM();
	resPnt_2_3D.zeroM();

	proection.oneM();

	posPnt_3D_1.matrix[0][0] = startPoint.x - center.x;
	posPnt_3D_1.matrix[1][0] = startPoint.y - center.y;
	posPnt_3D_1.matrix[2][0] = startPoint.z;
	posPnt_3D_1.matrix[3][0] = 1;
	
	// proection.oneM();
	proection.multi(posPnt_3D_1, resPnt_1_3D);

	float x = resPnt_1_3D.matrix[0][0];
	float y = resPnt_1_3D.matrix[1][0];
	float z = resPnt_1_3D.matrix[2][0];
	float tmp_t = resPnt_1_3D.matrix[3][0];

	point[0].y = y * Z_NEAR / z;
	point[0].x = x * Z_NEAR / z;

	resPnt_1_3D.zeroM();
	//=============================================================================
	posPnt_3D_1.matrix[0][0] = endPoint.x - center.x;
	posPnt_3D_1.matrix[1][0] = endPoint.y - center.y;
	posPnt_3D_1.matrix[2][0] = endPoint.z;
	posPnt_3D_1.matrix[3][0] = 1;

	proection.multi( posPnt_3D_1, resPnt_1_3D);

	x = resPnt_1_3D.matrix[0][0];
	y = resPnt_1_3D.matrix[1][0];
	z = resPnt_1_3D.matrix[2][0];
	tmp_t = resPnt_1_3D.matrix[3][0];

	point[1].y = y * Z_NEAR / z;
	point[1].x = x * Z_NEAR / z;

	for (int i = 0; i < 2; ++i) {
		point[i].x += center.x;
		point[i].y += center.y;
	}

	return point;
}
//=============================================================================
void Line_3D::displayLine_3D(HDC memDC, COLORREF color, pnt center) {
	Line tmp_ln;

	POINT* mass = convTo2D(center);

	tmp_ln.setStart(mass[0]);
	tmp_ln.setEnd(mass[1]);

	tmp_ln.brLine(memDC, color);
	
	delete mass;
}