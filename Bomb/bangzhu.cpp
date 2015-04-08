#include "bangzhu.h"
#include "gongxiang.h"

CButton5::CButton5(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton5::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->SetCurScene("��ʼ����");
	CursorQuit();
}

void CSceneBANGZHU::Init()
{
	PGF = CGameFrame::GetObject();
	LoadUI(new CButton5(0,550,80,588,"bmp\\����1.bmp","bmp\\����2.bmp"), "��ť5");
}
void CSceneBANGZHU::Run()
{
	if(m_TIME == 0)
		sound.play(_BZ);
	m_TIME += 1;
	if(m_TIME == 700)
		m_TIME = 0;
	PGF->GetGO()->DrawBmp(0, 0, 800, 600,0,0, "bmp\\enter.bmp");
	PGF->GetGO()->tDrawBmp(0, 0, 800, 600,0,0, "bmp\\����.bmp",RGB(255,255,255));
}

void CSceneBANGZHU::Enter()
{
	m_TIME = 0;
}
void CSceneBANGZHU::Quit()
{
	sound.Stop(_BZ);
}