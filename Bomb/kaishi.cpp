#include "kaishi.h"
#include "gongxiang.h"

CButton1::CButton1(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton1::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->GetScene("��ʼ����")->DS = false;
	CGameFrame::GetObject()->SetCurScene("��Ϸ����");
	CursorQuit();
}

CButton2::CButton2(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton2::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->GetScene("��ʼ����")->DS = true;
	CGameFrame::GetObject()->SetCurScene("��Ϸ����");
	CursorQuit();

}

CButton3::CButton3(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton3::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->SetCurScene("��������");
	CursorQuit();
}

CButton4::CButton4(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton4::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->Quit();
	CursorQuit();
}


void CSceneKAISHI::Init()
{
	PGF = CGameFrame::GetObject();
	Loadmusic();
	LoadUI(new CButton1(200,100,397,161,"bmp\\������Ϸ1.bmp","bmp\\������Ϸ2.bmp"), "��ť2");
	LoadUI(new CButton2(300,200,497,261,"bmp\\˫����Ϸ1.bmp","bmp\\˫����Ϸ2.bmp"), "��ť3");
	LoadUI(new CButton3(400,300,597,361,"bmp\\��Ϸ����1.bmp","bmp\\��Ϸ����2.bmp"), "��ť4");
	LoadUI(new CButton4(500,400,697,461,"bmp\\��Ϸ�˳�1.bmp","bmp\\��Ϸ�˳�2.bmp"), "��ť6");
}

void CSceneKAISHI::Enter()
{
	m_time = 0;
		PGF->GetGO()->SetTitleText("������");
}

void CSceneKAISHI::Run()
{
	if(m_time == 0)
		sound.play(_KS);
	m_time += 1;
	if(m_time == 800)
		m_time = 0;
	PGF->GetGO()->DrawBmp(0, 0, 800, 600,0,0, "bmp\\enter.bmp");
}

void CSceneKAISHI::Quit()
{
	sound.Stop(_KS);
}

void CSceneKAISHI::Loadmusic()
{
	sound.ReadFile(START, "sound1\\start.wav",DOSOUND);
	sound.ReadFile(WIN, "sound1\\win.wav",DOSOUND);
	sound.ReadFile(DEUCE, "sound1\\deuce.wav",DOSOUND);
	sound.ReadFile(LOST, "sound1\\lost.wav",DOSOUND);
	sound.ReadFile(LAY, "sound1\\lay.wav",DOSOUND);
	sound.ReadFile(DETONATION, "sound1\\detonation.wav",DOSOUND);
	sound.ReadFile(APPEAR, "sound1\\appear.wav",DOSOUND);
	sound.ReadFile(GET, "sound1\\get.wav",DOSOUND);
	sound.ReadFile(DIE, "sound1\\die.wav",DOSOUND);
	sound.ReadFile(_BJ, "sound1\\Pirate.mp3",BACKMUSIC);
	sound.ReadFile(_BZ, "sound1\\main.wav",BACKMUSIC);
	sound.ReadFile(_KS, "sound1\\back.wav",BACKMUSIC);
} 