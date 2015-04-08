#ifndef _SLIDER_H_
#define _SLIDER_H_
 
#include "UI.h"

class CSlider : public CUI
{
	int m_Move;
	const char* m_InBmp;
	const char* m_OutBmp;
	const char* m_MLBmp;
	const char* m_MRBmp;

public:
	CSlider(
		int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp,
		const char* MLBmp,
		const char* MRBmp);
	void CursorInMove(int x, int y);
	void CursorQuit();
	void Render();
};

#endif