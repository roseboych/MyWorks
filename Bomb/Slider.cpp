#include "Slider.h"
#include "GameFrame.h"
 
CSlider::CSlider(int x1, int y1, int x2, int y2,
				 const char* InBmp,
				 const char* OutBmp,
				 const char* MLBmp,
				 const char* MRBmp)
:
CUI(x1, y1, x2, y2),
m_Move(0),
m_InBmp(InBmp),
m_OutBmp(OutBmp),
m_MLBmp(MLBmp),
m_MRBmp(MRBmp)
{}
void CSlider::CursorInMove(int x, int y)
{
	if (x > 0 && x < 20)
		m_Move = 1;
	else if (x > 80 && x < 100)
		m_Move = 2;
	else if (x >= 20 && x <= 80)
		m_Move = 3;
}
void CSlider::CursorQuit()
{
	m_Move = 0;
}
void CSlider::Render()
{
	CGameFrame* pGF = CGameFrame::GetObject();

	switch (m_Move)
	{
	case 0:pGF->GetGO()->DrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_OutBmp);break;
	case 1:pGF->GetGO()->DrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_MLBmp);break;
	case 2:pGF->GetGO()->DrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_MRBmp);break;
	case 3:pGF->GetGO()->DrawBmp(x1, y1, x2 - x1, y2 - y1, 0, 0, m_InBmp);break;
	}
	char buf[4];
	_itoa_s(CScene::m_Alpha, buf, 4, 10);
	pGF->GetGO()->DrawTextA(buf, x1 + 30, y1 + 5, RGB(255, 0, 0), "Êý×Ö¿ò");
}