#ifndef _SCENE_H_
#define _SCENE_H_
 
#include "UI.h"
#include <string>
#include <map>
#include "sound\\soundmanage.h"

class CScene
{
public:
	std::map<std::string, CUI*> m_UI;
	CUI* m_KeyInputUI;
	static int m_Alpha;
	static bool DS;
	static CSoundManage sound;
	
	bool LoadUI(CUI* pUI, const char* ID);
	bool ReleaseUI(const char* ID);
	bool SetKeyInputUI(const char* ID = 0);

	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void Enter();
	virtual void Run();
	virtual void Quit();
	virtual void End();
};

#endif