#ifndef _GAME_H_
#define _GAME_H_

#include "GameFrame.h"
#include "Scene.h"
#include "gongxiang.h"
#include<time.h>
#include <iostream>
#include "Button.h"


class CButton6 : public CButton
{
public:
	CButton6(int x1, int y1, int x2, int y2,
		const char* InBmp,
		const char* OutBmp);
	void LButtonInUp(int x, int y);
};


class CSceneGAME : public CScene
{
	int    m_Player_cz;                  //玩家操作代号
	int    m_id;                         //操作玩家ID
	float   yl;                         //当前音量 
	int     m_time;                     //计时器
	bool    jy;                         //表示静音
	PLAYER	m_Player[PLAYER_NUM];		//	存放所有玩家信息
	bool	m_PlayerLay[PLAYER_NUM];	//	是否放炸弹
	int		m_LayX[PLAYER_NUM];			//	炸弹摆放位置x
	int		m_LayY[PLAYER_NUM];			//	炸弹摆放位置y
	int		m_Delay[PLAYER_NUM];		//	躲避危险的延时时间

	int		m_GameState;                //游戏状态
	int		m_GameCounter;              //游戏计数

	ITEM	m_ScreenItem[13][15];		//	存放背景信息
	list<MOVEBOMB>	m_bombKick;			//	存放踢出来的泡泡信息
	list<MOVEBOMB>	m_bombCast;			//	存放扔出来的泡泡信息

	int			m_Graph[MaxVertexNum][MaxVertexNum];	//	图的邻接矩阵，此处用二维数组表示
	int			m_Distance[MaxVertexNum];				//	源点到其它各顶点的最短距离
	int			m_Path[MaxVertexNum];					//	源点到其它各顶点的最短路径

	BOOL		m_bHelp;
	DWORD		m_timeNew;
	DWORD		m_timeOld;
	DWORD		m_counter;
	float		m_fps;
	CGameFrame* PGF;
	int	        test;
	BOOL		m_bWindowed;
public:

	void Init();
	void Enter();
	void Quit();
	void Run();


	void ResetMapInfor();
	void XR();
	void DoInput();	                //	处理玩家输入
	void DoInput1();	            //	检测玩家1输入
	void DoInput2();	            //	检测玩家2输入
	void DoMove();					//	处理角色移动
	void DoProperty();				//	处理道具
	void DoKick();					//	处理踢泡泡
	void DoCast();					//	处理扔泡泡
	void DoKill();					//	判断对方是否已被炸，若是，则经过时可立刻杀死对方
	void SetBlast(int i,int j);
	

	//AI部分
	bool FindSafety(int PlayerIndex);				//	查找一个安全的位置，找到后去躲藏
	bool FindPropertyForGet(int PlayerIndex);		//	查看道具的位置，若找到则去吃
	bool FindAdversaryForAttack(int PlayerIndex);	//	查找对手的位置，找到后去攻击
	bool FindLay(int PlayerIndex);					//	查找一个放置炸弹的位置(能炸对手就炸对手，否则炸物体，以便得到道具)
	bool IsDangerous(int player_x,int player_y);	//	检查当前是否有危险(输入参数为玩家的坐标)
	bool FindAddress(int PlayerIndex,int s);		//	查找一个放置
	void SetGraph(int PlayerIndex);					//	设置图
	int	 GetDestroyNumber(int PlayerIndex,int x,int y);	//	得到某个玩家在(x,y)处方下炸弹可以摧毁的物体数目
	void Dijkstra(int G[MaxVertexNum][MaxVertexNum], int D[MaxVertexNum], int P[MaxVertexNum],int s);	//	迪杰斯特拉算法

};

#endif