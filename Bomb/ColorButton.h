#ifndef _COLOR_BUTTON_H_
#define _COLOR_BUTTON_H_

#include "UI.h"

class CColorButton : public CUI
{
private:
	bool m_In;
	unsigned int m_InColor;
	unsigned int m_OutColor;

public:
	CColorButton(
		int x1, int y1, int x2, int y2,
		unsigned int incolor,
		unsigned int outcolor);
	void CursorEnter();
	void CursorQuit();
	void Render();
};

#endif