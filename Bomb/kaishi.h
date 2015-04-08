#ifndef _KAISHI_H_
#define _KAISHI_H_
 
#include "Button.h"
#include "Scene.h"
#include "GameFrame.h"

class CButton1 : public CButton
{
public:
	CButton1(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};

class CButton2 : public CButton
{
public:
	CButton2(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};

class CButton3 : public CButton
{
public:
	CButton3(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};

class CButton4 : public CButton
{
public:
	CButton4(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};
class CSceneKAISHI : public CScene
{
	int m_time;
	CGameFrame* PGF;
public:
	void Init();
	void Enter();
	void Run();
	void Quit();

	void Loadmusic();//º”‘ÿ“Ù¿÷
};

#endif