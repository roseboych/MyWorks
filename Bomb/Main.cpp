#include <windows.h>
#include "GameFrame.h"
#include "game.h"
#include "kaishi.h"
#include "bangzhu.h"
#include "logo.h"


//应用程序入口

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CGameFrame::SetClient(800, 600);
	CGameFrame* pGF = CGameFrame::GetObject();
	pGF->LoadScene(new CSceneGAME, "游戏场景");
	pGF->LoadScene(new CSceneKAISHI, "开始场景");
	pGF->LoadScene(new CSceneBANGZHU, "帮助场景");
	pGF->LoadScene(new CSceneLOGO, "LOGO场景");

	pGF->SetCurScene("LOGO场景");

	pGF->Run();

	pGF->End();

	return 1;
}