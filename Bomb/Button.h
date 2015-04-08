#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "UI.h"

class CButton : public CUI
{
private:
	bool m_In;
	const char* m_InBmp;
	const char* m_OutBmp;

public:
	CButton(
		int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void CursorEnter();
	void CursorQuit();
	void Render();
};

#endif