#include "logo.h"

void CSceneLOGO::Init()
{
	PGF = CGameFrame::GetObject();
	PGF->GetGO()->LoadDirectory("bmp");
	PGF->GetGO()->SetTitleText("������");
	r = 255;
}
void CSceneLOGO::Run()
{
	r -= 1;
	//PGF->GetGO()->DrawBmp(0, 0, 800, 600,0,0, "bmp\\LOGO.bmp");
	//if(PGF->GetGI()->Get(_GI_M_L) == _KS_UC)
	//	PGF->SetCurScene("��ʼ����");
	PGF->GetGO()->DrawAlphaBlend(0,0,800,600,0,0,"bmp\\LOGO.bmp",r);
	if(r < 0)
		PGF->SetCurScene("��ʼ����");
}
