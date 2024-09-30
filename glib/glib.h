/*
* -----------------------------------------
*      基本グラフィックスライブラリ       *
*         Visual Studio 2022              *
*         Win32プロジェクト版             *
* -----------------------------------------
*/

#include <math.h>

HDC ghdc;    // グラフィックス・デバイスコンテキスト
COLORREF color = RGB(0, 0, 255); // 青色

double WX1, WY1, WX2, WY2,      // ワールド座標
       VX1, VY1, VX2, VY2,      // ビュー座標
       FACTX, FACTY,          	// スケール
       ANGLE,               	// 現在角
       LPX, LPY;              	// 現在位置

void window(double x1, double y1, double x2, double y2)
{
	WX1 = x1; WY1 = y1; WX2 = x2; WY2 = y2;
	FACTX = (VX2 - VX1) / (WX2 - WX1);
	FACTY = (VY2 - VY1) / (WY2 - WY1);
}
void view(double x1, double y1, double x2, double y2)
{
	SetViewportExtEx(ghdc, (int)(x2 - x1), (int)(y2 - y1), NULL);
	SetViewportOrgEx(ghdc, (int)x1, (int)y1, NULL);
	HRGN hRgn = CreateRectRgn((int)x1, (int)y1, (int)x2 + 1, (int)y2 + 1);
	SelectClipRgn(ghdc, hRgn);		// クリップ領域の設定
	VX1 = x1; VY1 = y1; VX2 = x2; VY2 = y2;
	FACTX = (VX2 - VX1) / (WX2 - WX1);
	FACTY = (VY2 - VY1) / (WY2 - WY1);
}

void Ginit(HDC hdc)
{
	ghdc = hdc;
	HPEN hPen = CreatePen(PS_SOLID, 1, color);	// ペンの色   
	SelectObject(ghdc, hPen);
	LPX = 0; LPY = 0; ANGLE = 0;
	window(0, 0, 639, 399);
	view(0, 0, 639, 399);
}
void cls(void)
{
	RECT r;
	GetClipBox(ghdc, &r);
	FillRect(ghdc, &r, CreateSolidBrush(GetBkColor(ghdc)));
}
void line(double x1, double y1, double x2, double y2)
{
	int px1, py1, px2, py2;
	px1 = (int)((x1 - WX1)*FACTX);
	py1 = (int)((WY2 - y1)*FACTY);
	px2 = (int)((x2 - WX1)*FACTX);
	py2 = (int)((WY2 - y2)*FACTY);
	MoveToEx(ghdc, px1, py1, NULL);
	LineTo(ghdc, px2, py2);
	LPX = x2; LPY = y2;
}
void pset(double x, double y)
{
	int px, py;
	px = (int)((x - WX1)*FACTX);
	py = (int)((WY2 - y)*FACTY);
	SetPixel(ghdc, px, py,color);	
	LPX = x; LPY = y;
}
void move(double l)
{
	double x, y, rd = 3.1415927 / 180;
	x = l*cos(rd*ANGLE); y = l*sin(rd*ANGLE);
	line(LPX, LPY, LPX + x, LPY + y);
}
void moveto(double x, double y)
{
	line(LPX, LPY, x, y);
}
void setpoint(double x, double y)
{
	LPX = x; LPY = y;
}

#define setangle(a) ANGLE=(double)(a)
#define turn(a) ANGLE=fmod(ANGLE+(a),360.0)

#define ginit() Ginit(hdc)
