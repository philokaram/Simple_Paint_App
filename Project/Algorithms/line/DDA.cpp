#include <Windows.h>
int Round(double x)
{
	return (int)(x + 0.5);
}
void DrawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
	int dx = x2 - x1, dy = y2 - y1;
	SetPixel(hdc, x1, y1, c);
	if (abs(dx) >= abs(dy))
	{
		double m = (double)dy / dx;
		int x = x1;
		double y = y1;
		while (x < x2)
		{
			x++;
			y += m;
			SetPixel(hdc, x, Round(y), c);
		}
	}
	else {
		double mi = (double)dx / dy;
		int y = y1;
		double x = x1;
		while (y < y2)
		{
			y++;
			x += mi;
			SetPixel(hdc, Round(x), y, c);
		}

	}
}
