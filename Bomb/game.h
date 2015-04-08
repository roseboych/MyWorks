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
	int    m_Player_cz;                  //��Ҳ�������
	int    m_id;                         //�������ID
	float   yl;                         //��ǰ���� 
	int     m_time;                     //��ʱ��
	bool    jy;                         //��ʾ����
	PLAYER	m_Player[PLAYER_NUM];		//	������������Ϣ
	bool	m_PlayerLay[PLAYER_NUM];	//	�Ƿ��ը��
	int		m_LayX[PLAYER_NUM];			//	ը���ڷ�λ��x
	int		m_LayY[PLAYER_NUM];			//	ը���ڷ�λ��y
	int		m_Delay[PLAYER_NUM];		//	���Σ�յ���ʱʱ��

	int		m_GameState;                //��Ϸ״̬
	int		m_GameCounter;              //��Ϸ����

	ITEM	m_ScreenItem[13][15];		//	��ű�����Ϣ
	list<MOVEBOMB>	m_bombKick;			//	����߳�����������Ϣ
	list<MOVEBOMB>	m_bombCast;			//	����ӳ�����������Ϣ

	int			m_Graph[MaxVertexNum][MaxVertexNum];	//	ͼ���ڽӾ��󣬴˴��ö�ά�����ʾ
	int			m_Distance[MaxVertexNum];				//	Դ�㵽�������������̾���
	int			m_Path[MaxVertexNum];					//	Դ�㵽��������������·��

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
	void DoInput();	                //	�����������
	void DoInput1();	            //	������1����
	void DoInput2();	            //	������2����
	void DoMove();					//	�����ɫ�ƶ�
	void DoProperty();				//	�������
	void DoKick();					//	����������
	void DoCast();					//	����������
	void DoKill();					//	�ж϶Է��Ƿ��ѱ�ը�����ǣ��򾭹�ʱ������ɱ���Է�
	void SetBlast(int i,int j);
	

	//AI����
	bool FindSafety(int PlayerIndex);				//	����һ����ȫ��λ�ã��ҵ���ȥ���
	bool FindPropertyForGet(int PlayerIndex);		//	�鿴���ߵ�λ�ã����ҵ���ȥ��
	bool FindAdversaryForAttack(int PlayerIndex);	//	���Ҷ��ֵ�λ�ã��ҵ���ȥ����
	bool FindLay(int PlayerIndex);					//	����һ������ը����λ��(��ը���־�ը���֣�����ը���壬�Ա�õ�����)
	bool IsDangerous(int player_x,int player_y);	//	��鵱ǰ�Ƿ���Σ��(�������Ϊ��ҵ�����)
	bool FindAddress(int PlayerIndex,int s);		//	����һ������
	void SetGraph(int PlayerIndex);					//	����ͼ
	int	 GetDestroyNumber(int PlayerIndex,int x,int y);	//	�õ�ĳ�������(x,y)������ը�����Դݻٵ�������Ŀ
	void Dijkstra(int G[MaxVertexNum][MaxVertexNum], int D[MaxVertexNum], int P[MaxVertexNum],int s);	//	�Ͻ�˹�����㷨

};

#endif