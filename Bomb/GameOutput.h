#ifndef _GAME_OUTPUT_H_
#define _GAME_OUTPUT_H_

#include <windows.h>
#include <string>
#include <map>
  
#define _M_N 0
#define _M_H 1
#define _M_V 2
#define _M_HV 3

class CGameOutput
{
protected:
	HWND m_hWnd;
	int m_ClientW;
	int m_ClientH;
	HDC m_MainDC;
	HDC m_BackDC;
	HDC m_MirrorDC;
	HDC m_AlphaDC;
	std::map<std::string, HDC> m_Bmp;
	std::map<std::string, HFONT> m_Font;

public:
	CGameOutput(HWND hwnd);
	virtual ~CGameOutput();

	void BeginOutput();
	void EndOutput();

	bool LoadBmp(const char* bmp);
	bool ReleaseBmp(const char* bmp);
	bool DrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp);
	bool tDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, unsigned int tc = RGB(0, 255, 0));
	bool DrawDot(int x, int y, unsigned int c = RGB(0, 0, 0));
	bool DrawLine(int x1, int y1, int x2, int y2, unsigned int c = RGB(0, 0, 0));
	bool DrawRectangle(int x, int y, int w, int h, unsigned int pc = RGB(0, 0, 0), unsigned int bc = RGB(255, 255, 255));
	bool DrawEllipse(int x, int y, int w, int h, unsigned int pc = RGB(0, 0, 0), unsigned int bc = RGB(255, 255, 255));
	bool DrawAlphaBlend(int tm);
	bool DrawAlphaBlend(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, int tm);

	int LoadDirectory(const char* d);
	bool mDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, char m = _M_N);
	bool tmDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, unsigned int tc = RGB(0, 255, 0), char m = _M_N);
	bool CreateFont(int h, const char* n, const char* f);
	bool ReleaseFont(const char* f);
	bool DrawText(const char* t, int x, int y, unsigned int c = RGB(0, 0, 0), const char* f = 0);
	bool DrawText(const char* t, int x, int y, int w, int h, unsigned int c = RGB(0, 0, 0), const char* f = 0);
	bool SetTitleText(const char* t);
};

#endif