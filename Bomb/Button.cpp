#include "Button.h"
#include "GameFrame.h"

CButton::CButton(int x1, int y1, int x2, int y2,
						   const char* InBmp,
						   const char* OutBmp)
:
CUI(x1, y1, x2, y2),
m_In(false),
m_InBmp(InBmp),
m_OutBmp(OutBmp)
{}
void CButton::CursorEnter()
{
	m_In = true;
}
void CButton::CursorQuit()
{
	m_In = false;
}
void CButton::Render()
{
	CGameFrame* pGF = CGameFrame::GetObject();

	if (m_In)
		pGF->GetGO()->tDrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_InBmp,RGB(128,128,128));
	else
		pGF->GetGO()->tDrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_OutBmp,RGB(128,128,128));
}