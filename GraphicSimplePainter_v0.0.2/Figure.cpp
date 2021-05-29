#include "Figure.h"

void zero_z_buffer(float* z_buffer) {
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
		z_buffer[i] = MININT;
}

template <class T>
void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}
pnt operator*(pnt t1, float alpha) {
	pnt tmp = t1;

	tmp.x *= alpha;
	tmp.y *= alpha;
	tmp.z *= alpha;

	return tmp;
}
pnt operator-(pnt t1, pnt t2) {
	pnt tmp;
	
	tmp.x = t1.x - t2.x;
	tmp.y = t1.y - t2.y;
	tmp.z = t1.z - t2.z;

	return tmp;
}
pnt operator+(pnt t1, pnt t2) {
	pnt tmp;

	tmp.x = t1.x + t2.x;
	tmp.y = t1.y + t2.y;
	tmp.z = t1.z + t2.z;

	return tmp;
}

void triangle_render(pnt t0, pnt t1, pnt t2, HDC image, COLORREF color, float* zbuffer) {
	// not process degenerate triangles
	if (t0.y == t1.y && t0.y == t2.y) return;
	
	if (t0.y > t1.y) swap(t0, t1);
	if (t0.y > t2.y) swap(t0, t2);
	if (t1.y > t2.y) swap(t1, t2);

	int total_height = t2.y - t0.y;

	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		
		// careful: with above conditions no division by zero here
		float alpha = (float)i / total_height;
		// careful: with above conditions no division by zero here
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; 
		
		pnt A = t0 + (t2 - t0) * alpha;
		pnt B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		
		if (A.x > B.x) swap(A, B);

		for (int j = A.x; j <= B.x; j++) {
			float phi = B.x == A.x ? 1. : (float)(j - A.x) / (float)(B.x - A.x);
			pnt P = pnt(A) + (B - A) * phi;
			int idx = ceil(P.x) + ceil(P.y) * WIDTH;
			
			if((P.x < 500 && P.x > -1) && (P.y < 500 && P.y > -1))
				if (zbuffer[idx] < P.z){
					zbuffer[idx] = P.z;

					SetPixel( image, ceil(P.x), ceil(P.y), color);
				}
		}
	}
}

void triangular_pyramid::displayPyramid(HDC memDC, COLORREF color, pnt center) {
	pnt points_current[4];

	points_current[0] = lines[0].getPnts(0);
	points_current[1] = lines[0].getPnts(1);
	points_current[2] = lines[1].getPnts(1);
	points_current[3] = lines[2].getPnts(1);

	for (int i = 0; i < 4; ++i)
		points[i] = points_current[i];

	// center
	// in the future we need universal function
	center.x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
	center.y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;
	center.z = (points[0].z + points[1].z + points[2].z + points[3].z) / 4;

	for (int i = 0; i < 6; ++i)
		lines[i].displayLine_3D(memDC, color, center);

	float* z_buffer = new float[WIDTH * HEIGHT];
	zero_z_buffer(z_buffer);

	POINT* mass = lines[0].convTo2D(center);

	points_current[0].x = mass[0].x;
	points_current[0].y = mass[0].y;

	points_current[1].x = mass[1].x;
	points_current[1].y = mass[1].y;

	delete mass;
	mass = lines[1].convTo2D(center);

	points_current[2].x = mass[1].x;
	points_current[2].y = mass[1].y;

	delete mass;
	mass = lines[2].convTo2D(center);

	points_current[3].x = mass[1].x;
	points_current[3].y = mass[1].y;

	delete mass;

	if (color != RGB(255, 255, 255)) {
		triangle_render(points_current[1], points_current[2], points_current[3], memDC, RGB(255, 0, 0), z_buffer);
		triangle_render(points_current[0], points_current[2], points_current[3], memDC, RGB(0, 0, 255), z_buffer);
		triangle_render(points_current[0], points_current[1], points_current[3], memDC, RGB(100, 100, 100), z_buffer);
		triangle_render(points_current[0], points_current[1], points_current[2], memDC, RGB(0, 255, 0), z_buffer);
	}
	else {
		COLORREF white = RGB(255, 255, 255);

		// can be optimized
		triangle_render(points_current[1], points_current[2], points_current[3], memDC, white, z_buffer);
		triangle_render(points_current[0], points_current[2], points_current[3], memDC, white, z_buffer);
		triangle_render(points_current[0], points_current[1], points_current[2], memDC, white, z_buffer);
		triangle_render(points_current[0], points_current[1], points_current[3], memDC, white, z_buffer);
	}

	delete[] z_buffer;
}

void triangular_pyramid::calcNewPos(LPARAM lParam, char flCursor, char flVect, HDC memDC, POINT tmpDT) {
	pnt center;
	pnt points_current[4];

	points_current[0] = lines[0].getPnts(0);
	points_current[1] = lines[0].getPnts(1);
	points_current[2] = lines[1].getPnts(1);
	points_current[3] = lines[2].getPnts(1);

	for (int i = 0; i < 4; ++i)
		points[i] = points_current[i];

	// center
	// in the future we need universal function
	center.x = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
	center.y = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;
	center.z = (points[0].z + points[1].z + points[2].z + points[3].z) / 4;

	// fill with white
	displayPyramid(memDC, RGB(255, 255, 255), center);
	
	// calc new pose
	for (int i = 0; i < 6; ++i)
		lines[i].transformPoint(lParam, flCursor, flVect, tmpDT, center);

	// fill with black
	displayPyramid(memDC, RGB(100,200,150), center);
}