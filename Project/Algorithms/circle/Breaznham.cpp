#include <Windows.h>

void DrawPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc - y, yc - x, c);
}

void DrawCircleBres(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0, y = r, r2 = r * r, d = 1 - r, d1 = 3, d2 = 5 - (2 * r);
	DrawPoints(hdc, xc, yc, x, y, c);
	while (x < y) {
		if (d < 0) {
			d += d1;
			d2 += 2;
		}
		else {
			d += d2;
			d2 += 4;
			y--;
		}
		d1 += 2;
		x++;
		DrawPoints(hdc, xc, yc, x, y, c);
	}
}