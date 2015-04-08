#include "GameOutput.h"
#include <cassert>
#include <io.h>

//https://msdn.microsoft.com/zh-cn/library/vstudio/k7xzw3sa
#pragma comment(lib, "Msimg32.lib")

CGameOutput::CGameOutput(HWND hwnd)
:
m_hWnd(hwnd),
m_ClientW(0),
m_ClientH(0),
m_MainDC(0),
m_BackDC(0)
{
#ifdef _DEBUG
	GdiSetBatchLimit(1);
#endif

	if (IsWindow(m_hWnd))
	{
		RECT r;
		GetClientRect(m_hWnd, &r);
		m_ClientW = r.right - r.left;
		m_ClientH = r.bottom - r.top;

		m_MainDC = GetDC(m_hWnd);

		HBITMAP hbmp;

		m_BackDC = CreateCompatibleDC(m_MainDC);
		hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
		DeleteObject(SelectObject(m_BackDC, hbmp));
		DeleteObject(hbmp);

		m_MirrorDC = CreateCompatibleDC(m_MainDC);
		hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
		DeleteObject(SelectObject(m_MirrorDC, hbmp));
		DeleteObject(hbmp);

		m_AlphaDC = CreateCompatibleDC(m_MainDC);
		hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
		DeleteObject(SelectObject(m_AlphaDC, hbmp));
		DeleteObject(hbmp);

		SetBkMode(m_BackDC, TRANSPARENT);
	}
}

CGameOutput::~CGameOutput()
{
	if (m_hWnd)
	{
		std::map<std::string, HFONT>::iterator i;
		for (i = m_Font.begin(); i != m_Font.end(); ++i)
			DeleteObject(i->second);

		std::map<std::string, HDC>::iterator j;
		for (j = m_Bmp.begin(); j != m_Bmp.end(); ++j)
			DeleteDC(j->second);
		
		DeleteDC(m_AlphaDC);
		DeleteDC(m_MirrorDC);
		DeleteDC(m_BackDC);
		ReleaseDC(m_hWnd, m_MainDC);
	}
}

void CGameOutput::BeginOutput()
{
	BitBlt(m_BackDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, WHITENESS);
}

void CGameOutput::EndOutput()
{
	BitBlt(m_MainDC, 0, 0, m_ClientW, m_ClientH, m_BackDC, 0, 0, SRCCOPY);
}

bool CGameOutput::LoadBmp(const char* bmp)
{
	std::string s = bmp;
	if (m_Bmp.find(s) != m_Bmp.end())
		return false;

	HBITMAP hbmp = (HBITMAP)LoadImageA(0, s.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hbmp)
		return false;

	HDC hdc = CreateCompatibleDC(m_MainDC);
	DeleteObject(SelectObject(hdc, hbmp));
	DeleteObject(hbmp);

	m_Bmp.insert(std::pair<std::string, HDC>(s, hdc));

	return true;
}

bool CGameOutput::ReleaseBmp(const char* bmp)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	DeleteDC(i->second);
	m_Bmp.erase(i);

	return true;
}

bool CGameOutput::DrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	return 0 != BitBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, SRCCOPY);
}

bool CGameOutput::tDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, unsigned int tc)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	return 0 != TransparentBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, dw, dh, tc);
}

bool CGameOutput::DrawAlphaBlend(int tm)
{
	if (tm < 0 || tm > 255)
		return false;

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = tm;
	bf.AlphaFormat = 0;

	BitBlt(m_AlphaDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, BLACKNESS);
	return 0 != AlphaBlend(m_BackDC, 0, 0, m_ClientW, m_ClientH, m_AlphaDC, 0, 0, m_ClientW, m_ClientH, bf);
}

bool CGameOutput::DrawAlphaBlend(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, int tm)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end() || tm < 0 || tm > 255)
		return false;

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = tm;
	bf.AlphaFormat = 0;

	return 0 != AlphaBlend(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, dw, dh, bf);
}

bool CGameOutput::DrawDot(int x, int y, unsigned int c)
{
	return 1 != SetPixel(m_BackDC, x, y, c);
}

bool CGameOutput::DrawLine(int x1, int y1, int x2, int y2, unsigned int c)
{
	HPEN np = CreatePen(PS_SOLID, 1, c);
	HPEN op = (HPEN)SelectObject(m_BackDC, np);

	bool r = ((0 != MoveToEx(m_BackDC, x1, y1, 0)) && (0 != LineTo(m_BackDC, x2, y2)));

	SelectObject(m_BackDC, op);
	DeleteObject(np);

	return r;
}

bool CGameOutput::DrawRectangle(int x, int y, int w, int h, unsigned int pc, unsigned int bc)
{
	if (pc == RGB(0, 0, 0) && bc == RGB(255, 255, 255))
		return TRUE == Rectangle(m_BackDC, x, y, x + w - 1, y + h - 1);
	else
	{
		HPEN np = CreatePen(PS_SOLID, 1, pc);
		HBRUSH nb = CreateSolidBrush(bc);
		HPEN op = (HPEN)SelectObject(m_BackDC, np);
		HBRUSH ob = (HBRUSH)SelectObject(m_BackDC, nb);

		int r = Rectangle(m_BackDC, x, y, x + w, y + h);
		
		SelectObject(m_BackDC, ob);
		SelectObject(m_BackDC, op);
		DeleteObject(nb);
		DeleteObject(np);

		return 0 != r;
	}
}

bool CGameOutput::DrawEllipse(int x, int y, int w, int h, unsigned int pc, unsigned int bc)
{
	if (pc == RGB(0, 0, 0) && bc == RGB(255, 255, 255))
		return TRUE == Ellipse(m_BackDC, x, y, x + w - 1, y + h - 1);
	else
	{
		HPEN np = CreatePen(PS_SOLID, 1, pc);
		HBRUSH nb = CreateSolidBrush(bc);
		HPEN op = (HPEN)SelectObject(m_BackDC, np);
		HBRUSH ob = (HBRUSH)SelectObject(m_BackDC, nb);

		int r = Ellipse(m_BackDC, x, y, x + w, y + h);
		
		SelectObject(m_BackDC, ob);
		SelectObject(m_BackDC, op);
		DeleteObject(nb);
		DeleteObject(np);

		return 0 != r;
	}
}

int CGameOutput::LoadDirectory(const char* d)
{
	char dir[260];
	strcpy_s(dir, 260, d);
	strcat_s(dir, 260, "\\*");

	int r = 0;

	_finddata_t fd;
	intptr_t fr = _findfirst(dir, &fd);

	if (-1 != fr)
	{
		int go = 0;
		while (!go)
		{
			if ((fd.attrib & _A_SUBDIR))
			{
				if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0)
				{
					strcpy_s(dir, 260, d);
					strcat_s(dir, 260, "\\");
					strcat_s(dir, 260, fd.name);
					r += LoadDirectory(dir);
				}
			}
			else
			{
				strcpy_s(dir, 260, d);
				strcat_s(dir, 260, "\\");
				strcat_s(dir, 260, fd.name);
				r += LoadBmp(dir) ? 1 : 0;
			}
			go = _findnext(fr, &fd);
		}
	}

	return r;
}

bool CGameOutput::mDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, char m)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end() || m < _M_N || m > _M_HV)
		return false;

	bool r;
	switch (m)
	{
	case _M_N:
			return 0 != BitBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, SRCCOPY);
	case _M_H:
		{
			r = (0 != StretchBlt(m_MirrorDC, dw - 1, 0, -dw, dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	case _M_V:
		{
			r = (0 != StretchBlt(m_MirrorDC, 0, dh - 1, dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	case _M_HV:
		{
			r = (0 != StretchBlt(m_MirrorDC, dw - 1, dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	}
	
	return r ? 0 != BitBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, SRCCOPY) : r;
}

bool CGameOutput::tmDrawBmp(int dx, int dy, int dw, int dh, int sx, int sy, const char* bmp, unsigned int tc, char m)
{
	std::string s = bmp;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end() || m < _M_N || m > _M_HV)
		return false;

	bool r;
	switch (m)
	{
	case _M_N:
			return 0 != TransparentBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, dw, dh, tc);
	case _M_H:
		{
			r = (0 != StretchBlt(m_MirrorDC, dw - 1, 0, -dw, dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	case _M_V:
		{
			r = (0 != StretchBlt(m_MirrorDC, 0, dh - 1, dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	case _M_HV:
		{
			r = (0 != StretchBlt(m_MirrorDC, dw - 1, dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY));
			break;
		}
	}
	
	return r ? 0 != TransparentBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, dw, dh, tc) : r;
}

bool CGameOutput::CreateFont(int h, const char* n, const char* f)
{
	std::string s = f;
	if (m_Font.find(s) != m_Font.end())
		return false;

	LOGFONT logfont = {};
	logfont.lfHeight = h;
	logfont.lfCharSet = DEFAULT_CHARSET;
	strcpy_s(logfont.lfFaceName, LF_FACESIZE, n);

	HFONT hfont = CreateFontIndirect(&logfont);

	m_Font.insert(std::pair<std::string, HFONT>(s, hfont));

	return true;
}

bool CGameOutput::ReleaseFont(const char* f)
{
	std::string s = f;
	std::map<std::string, HFONT>::iterator i = m_Font.find(s);
	if (i == m_Font.end())
		return false;

	DeleteObject(i->second);
	m_Font.erase(i);

	return true;
}

bool CGameOutput::DrawText(const char* t, int x, int y, unsigned int c, const char* f)
{
	int r;

	if (f)
	{
		std::string s = f;
		std::map<std::string, HFONT>::iterator i = m_Font.find(s);
		if (i != m_Font.end())
		{
			HFONT of = (HFONT)SelectObject(m_BackDC, i->second);

			if (c == RGB(0, 0, 0))
			{
				r = TextOut(m_BackDC, x, y, t, (int)strlen(t));
				SelectObject(m_BackDC, of);
			}
			else
			{
				unsigned int oc = SetTextColor(m_BackDC, c);
				r = TextOut(m_BackDC, x, y, t, (int)strlen(t));
				SelectObject(m_BackDC, of);
				SetTextColor(m_BackDC, oc);
			}
		}
		else
			r = 0;
	}
	else
	{
		if (c == RGB(0, 0, 0))
			r = TextOut(m_BackDC, x, y, t, (int)strlen(t));
		else
		{
			unsigned int oc = SetTextColor(m_BackDC, c);
			r = TextOut(m_BackDC, x, y, t, (int)strlen(t));
			SetTextColor(m_BackDC, oc);
		}
	}

	return 0 != r;
}

bool CGameOutput::DrawText(const char* t, int x, int y, int w, int h, unsigned int c , const char* f)
{
	int r;
	RECT rect = {x, y, x + w - 1, y + h - 1};

	if (f)
	{
		std::string s = f;
		std::map<std::string, HFONT>::iterator i = m_Font.find(s);
		if (i != m_Font.end())
		{
			HFONT of = (HFONT)SelectObject(m_BackDC, i->second);

			if (c == RGB(0, 0, 0))
			{
				r = ::DrawText(m_BackDC, t, (int)strlen(t), &rect, DT_WORDBREAK);
				SelectObject(m_BackDC, of);
			}
			else
			{
				unsigned int oc = SetTextColor(m_BackDC, c);
				r = ::DrawText(m_BackDC, t, (int)strlen(t), &rect, DT_WORDBREAK);
				SelectObject(m_BackDC, of);
				SetTextColor(m_BackDC, oc);
			}
		}
		else
			r = 0;
	}
	else
	{
		if (c == RGB(0, 0, 0))
			r = ::DrawText(m_BackDC, t, (int)strlen(t), &rect, DT_WORDBREAK);
		else
		{
			unsigned int oc = SetTextColor(m_BackDC, c);
			r = ::DrawText(m_BackDC, t, (int)strlen(t), &rect, DT_WORDBREAK);
			SetTextColor(m_BackDC, oc);
		}
	}

	return 0 != r;
}

bool CGameOutput::SetTitleText(const char* t)
{
	return 0 != SetWindowText(m_hWnd, t);
}