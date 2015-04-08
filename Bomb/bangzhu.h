#ifndef _BANGZHU_H_
#define _BANGZHU_H_

#include "Button.h"
#include "Scene.h"
#include "GameFrame.h"
 
class CButton5 : public CButton
{
public:
	CButton5(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};

class CSceneBANGZHU : public CScene
{
	int m_TIME;
	CGameFrame* PGF;
public:
	void Init();
	void Enter();
	void Quit();
	void Run();
};

#endif