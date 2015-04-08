#ifndef _LOGO_H_
#define _LOGO_H_

#include "Scene.h"
#include "GameFrame.h"

class CSceneLOGO : public CScene
{
	CGameFrame* PGF;
	int r;
public:
	void Init();
	void Run(); 
};
#endif