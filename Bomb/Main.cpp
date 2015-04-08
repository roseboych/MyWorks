#include <windows.h>
#include "GameFrame.h"
#include "game.h"
#include "kaishi.h"
#include "bangzhu.h"
#include "logo.h"


//Ӧ�ó������

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGameFrame::SetClient(800, 600);
	CGameFrame* pGF = CGameFrame::GetObject();
	pGF->LoadScene(new CSceneGAME, "��Ϸ����");
	pGF->LoadScene(new CSceneKAISHI, "��ʼ����");
	pGF->LoadScene(new CSceneBANGZHU, "��������");
	pGF->LoadScene(new CSceneLOGO, "LOGO����");

	pGF->SetCurScene("LOGO����");

	pGF->Run();

	pGF->End();

	return 1;
}