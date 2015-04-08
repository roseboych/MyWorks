#include "game.h"

CButton6::CButton6(int x1, int y1, int x2, int y2,
				   const char* InBmp1,
				   const char* OutBmp1)
				   :
CButton(x1, y1, x2, y2, InBmp1, OutBmp1)
{}

void CButton6::LButtonInUp(int x, int y)
{
	CGameFrame::GetObject()->Quit();
	CursorQuit();
}


void CSceneGAME::Enter()
{
	sound.SetIsMute(false);
	ResetMapInfor();
	m_bHelp = TRUE;
	if(DS)
		PGF->GetGO()->SetTitleText("������---��Ҷ�ս");
	else
		PGF->GetGO()->SetTitleText("������---�˻���ս");
	jy = true;
	m_time = 0;
	sound.play(START);
	yl = _YL_;
	sound.SetVolume(yl);
}
void CSceneGAME::Quit()
{
	sound.Stop(_BJ);
}

void CSceneGAME::Init()
{
	PGF = CGameFrame::GetObject();
	LoadUI(new CButton6(650,560,780,590,"bmp\\kongjian2.bmp","bmp\\kongjian1.bmp"), "��ť1");
}
void CSceneGAME::Run()
{
	if(m_time == 5)
	{
		sound.play(_BJ);
		sound.SetVolume(yl);
	}
	m_time += 1;
	if(m_time >= 250 * 1000 / 20)
		m_time = 0;
	if (m_GameCounter == 0 && m_GameState != GAME_FREE && m_GameState != GAME_START )
	{
		sound.play(START);
		ResetMapInfor();
	}


	RECT	rect;
	int		i;

	//	1.�߼�������
	if (m_GameState == GAME_FREE || m_GameState == GAME_START)
	{
		DoInput1();					//	������
		DoMove();					//	�����ɫ�ƶ�
		DoProperty();				//	�������
		DoKill();
	}


	for (i=0;i<PLAYER_NUM;i++)		//	�ж��Ƿ�ը
	{
		if (m_Player[i].type != TYPE_NULL && m_Player[i].status != STATUS_DIED && m_Player[i].status != STATUS_MOVE)
		{
			if (m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].type == _BZ_)
			{
				m_Player[i].status = STATUS_DIED;
				m_Player[i].counter = 400;
			}
		}
	}

	//	�ж���Ϸ���
	if (m_GameState == GAME_FREE)
	{
		int		computerdie = false;
		int		userdie = false;
		for (i=0;i<PLAYER_NUM;i++)
		{
			if (m_Player[i].type == TYPE_COMPUTER && m_Player[i].status == STATUS_DIED && m_Player[i].counter < 60)
			{
				computerdie = true;
			}
		}
		if (m_Player[0].status == STATUS_DIED && m_Player[0].counter < 60)
		{
			userdie = true;
		}
		if (computerdie && userdie)
		{
			m_GameState = GAME_DEUCE;
			m_GameCounter = DISPLAYCOUNTER;
		}
		else	if (computerdie && !userdie)
		{
			m_GameState = GAME_WIN;
			m_GameCounter = DISPLAYCOUNTER;
		}
		else	if (!computerdie && userdie)
		{
			m_GameState = GAME_LOST;
			m_GameCounter = DISPLAYCOUNTER;
		}
	}

	XR();
	//	�����߳���������
	DoKick();

	//	�����ӳ���������
	DoCast();


	if (m_GameCounter>0)
	{
		rect.left = 0;
		rect.top = (m_GameState-1)*80;
		rect.right = 240;
		rect.bottom = 80+(m_GameState-1)*80;
		if(DS)
			PGF->GetGO()->tDrawBmp(200, 100,rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\word1.bmp",RGB(255,0,255));
		else
			PGF->GetGO()->tDrawBmp(200, 100,rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\word.bmp",RGB(255,0,255));
		m_GameCounter--;
		if (m_GameCounter == 0 && m_GameState == GAME_START)
		{
			m_GameState = GAME_FREE;
		}

	}
}

void CSceneGAME::ResetMapInfor()
{

	m_bombKick.clear();
	m_bombCast.clear();
	srand((unsigned int)time(0));

	for(int i=0;i<_LJ_H_;i++)
	{
		for(int j=0;j<_LJ_W_;j++)
		{
			m_ScreenItem[i][j].tag = -1;				//	��ʾ�޵���
			if (rand()%100>_XZ_JL_)
			{
				m_ScreenItem[i][j].type = _KG_;			//	0��ʾ��
			}
			else
			{
				if (rand()%100<_DJ_JL_)						//	30% �ļ���
				{
					m_ScreenItem[i][j].tag = rand()%_DJ_ZL_;
				}

				if (rand()%100<_MK_JL_)
				{
					m_ScreenItem[i][j].type = _MK_;		//	1��ʾľ��
				}
				else
				{
					m_ScreenItem[i][j].type = _XZ_;		//	2��ʾ����
				}
			}
		}
	}

	m_ScreenItem[1][1].type = _MZ_;						//	3��ʾľ׮
	m_ScreenItem[1][13].type = _MZ_;
	m_ScreenItem[11][1].type = _MZ_;
	m_ScreenItem[11][13].type = _MZ_;

	m_ScreenItem[6][6].type = _QZ_;						//	4,5,6��ʾ������
	m_ScreenItem[6][6].status = 0;
	m_ScreenItem[6][7].type = _QZ_;
	m_ScreenItem[6][7].status = 1;
	m_ScreenItem[6][8].type = _QZ_;
	m_ScreenItem[6][8].status = 2;

	m_ScreenItem[0][1].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[0][1].tag = -1;
	m_ScreenItem[0][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[0][2].tag = -1;
	m_ScreenItem[1][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[1][2].tag = -1;
	m_ScreenItem[2][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[2][2].tag = -1;
	  
	m_ScreenItem[0][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[0][12].tag = -1;
	m_ScreenItem[1][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[1][12].tag = -1;
	m_ScreenItem[2][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[2][12].tag = -1;
	m_ScreenItem[0][13].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[0][13].tag = -1;

	m_ScreenItem[10][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[10][2].tag = -1;
	m_ScreenItem[11][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[11][2].tag = -1;
	m_ScreenItem[12][2].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[12][2].tag = -1;
	m_ScreenItem[12][1].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[12][1].tag = -1;

	m_ScreenItem[10][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[10][12].tag = -1;
	m_ScreenItem[11][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[11][12].tag = -1;
	m_ScreenItem[12][12].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[12][12].tag = -1;
	m_ScreenItem[10][13].type = _KG_;						//	Ԥ��4���յ�λ��
	m_ScreenItem[10][13].tag = -1;

	for (int i=0;i<PLAYER_NUM;i++)
	{
		m_Player[i].type = TYPE_NULL;
		m_PlayerLay[i] = false;
	}

	//	������Ϣ
	m_Player[0].bomb = 1;
	m_Player[0].cast = false;
	m_Player[0].kick = false;
	m_Player[0].pin = false;
	m_Player[0].counter = 0;
	m_Player[0].way = WAY_DOWN;
	m_Player[0].step = 8;
	m_Player[0].speed = 1;
	m_Player[0].frame = 4;
	m_Player[0].status = STATUS_FREE;
	m_Player[0].power = 1;
	m_Player[0].type = TYPE_USER;
	m_Player[0].presswaykey = 0;
	m_Player[0].image = 2;

	m_Player[1].bomb = 1;
	m_Player[1].cast = false;
	m_Player[1].kick = false;
	m_Player[1].pin = false;
	m_Player[1].counter = 0;
	m_Player[1].way = WAY_DOWN;
	m_Player[1].step = 8;
	m_Player[1].speed = 1;
	m_Player[1].frame = 4;
	m_Player[1].status = STATUS_FREE;
	m_Player[1].power = 1;
	m_Player[1].type = TYPE_COMPUTER;
	m_Player[1].presswaykey = 0;
	m_Player[1].image = 1;

	POINT	point[4]={{80,80},{480,80},{480,400},{80,400}};

	int	pos = rand()%4;
	m_Player[0].x = point[pos].x;
	m_Player[0].y = point[pos].y;

	pos =(pos+2)%4;						//	ȡ�Խ�λ��
	m_Player[1].x = point[pos].x;
	m_Player[1].y = point[pos].y;

	m_GameState = GAME_START;
	m_GameCounter = DISPLAYCOUNTER;

	m_timeNew = m_timeOld;
	m_counter = 0;

	for(int i = 0; i < _LJ_H_; ++i)
	{
		for(int j = 0; j < _LJ_W_; ++j)
		{
			if(m_ScreenItem[i][j].type == _XZ_)
			{
				m_ScreenItem[i][j].xoffset = m_ScreenItem[i][j].yoffset = 0;
			}
		}
	}
}

void CSceneGAME::DoInput()
{
	if (m_Player_cz == _PLAY_CTRL_)
	{
		if (m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].type != _PP_)
		{
			if (m_Player[m_id].bomb != 0)
			{
				m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].type = _PP_;
				m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].counter = 200;
				m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].status = m_Player[m_id].power;
				m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].who = m_id;
				m_Player[m_id].bomb--;
				sound.play(LAY);
			}
		}
		else
		{
			if (m_Player[m_id].cast && m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].counter<240)
			{
				m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].type = _KG_;
				MOVEBOMB	move;
				move.mode = 1;
				move.way = m_Player[m_id].way;
				move.x = (m_Player[m_id].x+20)/40*40;
				move.y = (m_Player[m_id].y+20)/40*40;
				move.power = m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].status;
				move.counter = m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].counter;
				move.offset = 0;
				move.addorsub = true;
				move.who = m_ScreenItem[(m_Player[m_id].y+20)/40][(m_Player[m_id].x+20)/40].who;
				m_bombCast.push_back(move);
			}
		}
	}

	//if (PGF->GetGI()->Get(_GI_K_L_SHIFT) == _KS_UC  && m_Player[m_id].pin == true && m_Player[m_id].status == STATUS_DIED && m_Player[m_id].counter > 60)
	//{
	//	m_Player[m_id].pin = false;
	//	m_Player[m_id].status = STATUS_FREE;
	//}

	int	done;
	if (m_Player_cz == _PLAY_UP_ && (m_Player[m_id].status == STATUS_FREE) && (m_Player[m_id].y != 0))
	{
		done = CAN_MOVE;
		m_Player[m_id].way = WAY_UP;
		if (m_Player[m_id].y % 40 == 0)	//	�ж��赲
		{
			done = CAN_BLOCK;
			//	��ǰ��Ϊ��
			if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _BZ_ ))
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	ֱ����ǰ��
				}
				else	//	���Ϸ��ж���
				{
					if (m_Player[m_id].x%40 == 0)
					{
						done = CAN_MOVE;		//	ֱ����ǰ��
					}
					else if (m_Player[m_id].x%40 <= 30)
					{
						m_Player[m_id].way = WAY_LEFT;
						done = CAN_MOVE;		//	�����ƫ��
					}
					else if (m_Player[m_id].x%40 > 30)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	��ǰ���ж���
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].x%40 == 0)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].x%40 >= 10)
					{
						m_Player[m_id].way = WAY_RIGHT;
						done = CAN_MOVE;		//	���ұ�ƫ��
					}
					else if (m_Player[m_id].x%40 < 10)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
				else	//	���Ϸ��ж���
				{
					//	�˴����鷳��4�����:	1.��߿����ƶ����ұ߿����ƶ�	���x%40<20�����ƣ���������
					//							2.��߿����ƶ����ұ߲����ƶ�	���x%40<=30������
					//							3.��߲����ƶ����ұ߿����ƶ�	���x%40>=10������
					//							4.��߲����ƶ����ұ߲����ƶ�	�����ƶ�
					if (((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 < 20)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	�����ƫ��
						}
						else
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}

					if (((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& !((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 <= 30)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	�����ƫ��
						}
					}

					if ( !((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 >= 10)
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}
				}
			}
		}
		test = done;
		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	�����ƶ���־
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter�˴���ʾ��Ҫ������
			if (m_Player[m_id].step % m_Player[m_id].speed != 0)
			{
				m_Player[m_id].counter++;
			}
		}
		else	if (done == CAN_PUSH)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>40)
			{
				//	���������ӱ�־
				if (m_Player[m_id].x %40 < 10)
				{
					if (((m_Player[m_id].y-40)/40 != 0) && ((m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].counter = 40;
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].way = m_Player[m_id].way;
					}
				}
				else
				{
					if (((m_Player[m_id].y-40)/40 != 0) && ((m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].counter = 40;
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].way = m_Player[m_id].way;
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		else	if (done == CAN_KICK && m_Player[m_id].kick)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>20)
			{
				//	���������ݱ�־
				if (m_Player[m_id].x %40 < 10)
				{
					if (((m_Player[m_id].y-40)/40 != 0) && ((m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = m_Player[m_id].x/40*40;
						move.y = (m_Player[m_id].y-40)/40*40;
						move.counter = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].counter;
						move.power = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].status;
						move.who = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].who;
						m_bombKick.push_back(move);
					}
				}
				else
				{
					if (((m_Player[m_id].y-40)/40 != 0) && ((m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y-80)/40][m_Player[m_id].x/40+1].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40+1)*40;
						move.y = (m_Player[m_id].y-40)/40*40;
						move.counter = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].counter;
						move.power = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].status;
						move.who = m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].who;
						m_bombKick.push_back(move);
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		return;
	}
	if (m_Player_cz == _PLAY_DOWN_ && (m_Player[m_id].status == STATUS_FREE) && (m_Player[m_id].y != 12*40))
	{
		done = CAN_MOVE;
		m_Player[m_id].way = WAY_DOWN;
		if (m_Player[m_id].y % 40 == 0)	//	�ж��赲
		{
			done = CAN_BLOCK;
			//	��ǰ��Ϊ��
			if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _BZ_ ))
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	ֱ����ǰ��
				}
				else	//	���Ϸ��ж���
				{
					if (m_Player[m_id].x%40 == 0)
					{
						done = CAN_MOVE;		//	ֱ����ǰ��
					}
					else if (m_Player[m_id].x%40 <= 30)
					{
						m_Player[m_id].way = WAY_LEFT;
						done = CAN_MOVE;		//	�����ƫ��
					}
					else if (m_Player[m_id].x%40 > 30)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type)
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_)
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	��ǰ���ж���
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].x%40 == 0)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].x%40 >= 10)
					{
						m_Player[m_id].way = WAY_RIGHT;
						done = CAN_MOVE;		//	���ұ�ƫ��
					}
					else if (m_Player[m_id].x%40 < 10)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
				else	//	���Ϸ��ж���
				{
					//	�˴����鷳��4�����:	1.��߿����ƶ����ұ߿����ƶ�	���x%40<20�����ƣ���������
					//							2.��߿����ƶ����ұ߲����ƶ�	���x%40<=30������
					//							3.��߲����ƶ����ұ߿����ƶ�	���x%40>=10������
					//							4.��߲����ƶ����ұ߲����ƶ�	�����ƶ�
					if (((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 < 20)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;	//	�����ƫ��
						}
						else
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;	//	���ұ�ƫ��
						}
					}

					if (((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& !((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 <= 30)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	�����ƫ��
						}
					}

					if ( !((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 >= 10)
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}
				}
			}
		}
		test = done;
		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	�����ƶ���־
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter�˴���ʾ��Ҫ������
			if (m_Player[m_id].step % m_Player[m_id].speed != 0)
			{
				m_Player[m_id].counter++;
			}
		}
		else	if (done == CAN_PUSH)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>40)
			{
				//	���������ӱ�־
				if (m_Player[m_id].x %40 < 10)
				{
					if (((m_Player[m_id].y-40)/40 != 11*40 ) && ((m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].counter = 40;
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].way = m_Player[m_id].way;
					}
				}
				else
				{
					if (((m_Player[m_id].y-40)/40 != 11*40 ) && ((m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].counter = 40;
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].way = m_Player[m_id].way;
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		else	if (done == CAN_KICK && m_Player[m_id].kick)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>20)
			{
				//	���������ݱ�־
				if (m_Player[m_id].x %40 < 10)
				{
					if (((m_Player[m_id].y-40)/40 != 11*40 ) && ((m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = m_Player[m_id].x/40*40;
						move.y = (m_Player[m_id].y+40)/40*40;
						move.counter = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].counter;
						move.power = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].status;
						move.who = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].who;
						m_bombKick.push_back(move);
					}
				}
				else
				{
					if (((m_Player[m_id].y-40)/40 != 11*40 ) && ((m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _KG_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _DJ_) || (m_ScreenItem[(m_Player[m_id].y+80)/40][m_Player[m_id].x/40+1].type == _BZ_)))
					{
						m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40+1)*40;
						move.y = (m_Player[m_id].y+40)/40*40;
						move.counter = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].counter;
						move.power = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].status;
						move.who = m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].who;
						m_bombKick.push_back(move);
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		return;
	}
	if (m_Player_cz == _PLAY_LEFT_ && (m_Player[m_id].status == STATUS_FREE) && (m_Player[m_id].x != 0))
	{
		done = CAN_MOVE;
		m_Player[m_id].way = WAY_LEFT;
		if (m_Player[m_id].x % 40 == 0)	//	�ж��赲
		{
			done = CAN_BLOCK;
			//	��ǰ��Ϊ��
			if ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _BZ_ ))
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	ֱ����ǰ��
				}
				else	//	���Ϸ��ж���
				{
					if (m_Player[m_id].y%40 == 0)
					{
						done = CAN_MOVE;		//	ֱ����ǰ��
					}
					else if (m_Player[m_id].y%40 <= 30)
					{
						m_Player[m_id].way = WAY_UP;
						done = CAN_MOVE;		//	�����ƫ��
					}
					else if (m_Player[m_id].y%40 > 30)
					{
						if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	��ǰ���ж���
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _BZ_ ))
				{

					if (m_Player[m_id].y%40 == 0)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
						{
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].y%40 >= 10)
					{
						m_Player[m_id].way = WAY_DOWN;
						done = CAN_MOVE;		//	���ұ�ƫ��
					}
					else if ( m_Player[m_id].y % 40 < 10)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
				else	//	���Ϸ��ж���
				{
					//	�˴����鷳��4�����:	1.��߿����ƶ����ұ߿����ƶ�	���x%40<20�����ƣ���������
					//							2.��߿����ƶ����ұ߲����ƶ�	���x%40<=30������
					//							3.��߲����ƶ����ұ߿����ƶ�	���x%40>=10������
					//							4.��߲����ƶ����ұ߲����ƶ�	�����ƶ�
					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 < 20)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	�����ƫ��
						}
						else
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}

					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& !((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 <= 30)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	�����ƫ��
						}
					}

					if ( !((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 >= 10)
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}
				}
			}
		}
		test = done;

		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	�����ƶ���־
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter�˴���ʾ��Ҫ������
			if (m_Player[m_id].step % m_Player[m_id].speed != 0)
			{
				m_Player[m_id].counter++;
			}
		}
		else	if (done == CAN_PUSH)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>40)
			{
				//	���������ӱ�־
				if (m_Player[m_id].y %40 < 10)
				{
					if (((m_Player[m_id].x-40)/40 != 0) && ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].counter = 40;
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].way = m_Player[m_id].way;
					}
				}
				else
				{
					if (((m_Player[m_id].x-40)/40 != 0) && ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].counter = 40;
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].way = m_Player[m_id].way;
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		else	if (done == CAN_KICK && m_Player[m_id].kick)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>20)
			{
				//	���������ӱ�־
				if (m_Player[m_id].y %40 < 10)
				{
					if (((m_Player[m_id].x-40)/40 != 0) && ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40-1)*40;
						move.y = (m_Player[m_id].y)/40*40;
						move.counter = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].counter;
						move.power = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].status;
						move.who = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].who;
						m_bombKick.push_back(move);

					}
				}
				else
				{
					if (((m_Player[m_id].x-40)/40 != 0) && ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40)*40;
						move.y = (m_Player[m_id].y+40)/40*40;
						move.counter = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].counter;
						move.power = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].status;
						move.who = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40].who;
						m_bombKick.push_back(move);

					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}

		return;
	}
	if (m_Player_cz == _PLAY_RIGHT_ && (m_Player[m_id].status == STATUS_FREE) && (m_Player[m_id].x != 14*40))
	{
		done = CAN_MOVE;
		m_Player[m_id].way = WAY_RIGHT;
		if (m_Player[m_id].x % 40 == 0)	//	�ж��赲
		{
			done = CAN_BLOCK;
			//	��ǰ��Ϊ��
			if ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _BZ_ ))
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	ֱ����ǰ��
				}
				else	//	���Ϸ��ж���
				{
					if (m_Player[m_id].y%40 == 0)
					{
						done = CAN_MOVE;		//	ֱ����ǰ��
					}
					else if (m_Player[m_id].y%40 <= 30)
					{
						m_Player[m_id].way = WAY_UP;
						done = CAN_MOVE;		//	�����ƫ��
					}
					else if (m_Player[m_id].y%40 > 30)
					{
						if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	��ǰ���ж���
			{
				//	���Ϸ�Ϊ��	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].y%40 == 0)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].y%40 >= 10)
					{
						m_Player[m_id].way = WAY_DOWN;
						done = CAN_MOVE;		//	���ұ�ƫ��
					}
					else if (m_Player[m_id].y%40 < 10)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	���Ϸ������ӣ���
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	���Ϸ������ݣ���
							done = CAN_KICK;
						}
					}
				}
				else	//	���Ϸ��ж���
				{
					//	�˴����鷳��4�����:	1.��߿����ƶ����ұ߿����ƶ�	���x%40<20�����ƣ���������
					//							2.��߿����ƶ����ұ߲����ƶ�	���x%40<=30������
					//							3.��߲����ƶ����ұ߿����ƶ�	���x%40>=10������
					//							4.��߲����ƶ����ұ߲����ƶ�	�����ƶ�
					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 < 20)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	�����ƫ��
						}
						else
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}

					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& !((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
							{
								//	���Ϸ������ӣ���
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
							{
								//	���Ϸ������ݣ���
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 <= 30)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	�����ƫ��
						}
					}

					if ( !((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 >= 10)
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	���ұ�ƫ��
						}
					}
				}
			}
		}
		test = done;

		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	�����ƶ���־
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter�˴���ʾ��Ҫ������
			if (m_Player[m_id].step % m_Player[m_id].speed != 0)
			{
				m_Player[m_id].counter++;
			}
		}
		else	if (done == CAN_PUSH)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>40)
			{
				//	���������ӱ�־
				if (m_Player[m_id].y %40 < 10)
				{
					if (((m_Player[m_id].x-40)/40 != 13*40) && ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].counter = 40;
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].way = m_Player[m_id].way;
					}
				}
				else
				{
					if (((m_Player[m_id].x-40)/40 != 13*40) && ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].counter = 40;
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].way = m_Player[m_id].way;
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		else	if (done == CAN_KICK && m_Player[m_id].kick)
		{
			m_Player[m_id].presswaykey++;
			if (m_Player[m_id].presswaykey>20)
			{
				//	���������ӱ�־
				if (m_Player[m_id].y %40 < 10)
				{
					if (((m_Player[m_id].x-40)/40 != 13*40) && ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40+1)*40;
						move.y = (m_Player[m_id].y)/40*40;
						move.counter = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].counter;
						move.power = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].status;
						move.who = m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].who;
						m_bombKick.push_back(move);
					}
				}
				else
				{
					if (((m_Player[m_id].x-40)/40 != 13*40) && ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _KG_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _DJ_) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+2].type == _BZ_)))
					{
						m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type = _KG_;
						MOVEBOMB	move;
						move.mode = 0;
						move.way = m_Player[m_id].way;
						move.x = (m_Player[m_id].x/40+1)*40;
						move.y = (m_Player[m_id].y+40)/40*40;
						move.counter = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].counter;
						move.power = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].status;
						move.who = m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].who;
						m_bombKick.push_back(move);
					}
				}
				m_Player[m_id].presswaykey = 0;
			}
		}
		return;
	}
}

void CSceneGAME::DoMove()
{
	static	int	step = 0;
	step++;
	if (step == 40)
	{
		step = 0;
	}

	for (int PlayerIndex=0;PlayerIndex<PLAYER_NUM;PlayerIndex++)
	{

		if	(m_Player[PlayerIndex].status == STATUS_MOVE && m_Player[PlayerIndex].type != TYPE_NULL)
		{
			int	offset = m_Player[PlayerIndex].speed;
			if	((m_Player[PlayerIndex].counter == 1) && (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed !=0))
			{
				offset = m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed;
			}

			switch(m_Player[PlayerIndex].way)
			{
			case WAY_UP:
				m_Player[PlayerIndex].y -= offset;
				break;
			case WAY_DOWN:
				m_Player[PlayerIndex].y += offset;
				break;
			case WAY_LEFT:
				m_Player[PlayerIndex].x -= offset;
				break;
			case WAY_RIGHT:
				m_Player[PlayerIndex].x += offset;
				break;
			default:
				break;
			}

			m_Player[PlayerIndex].frame = step/10;

			m_Player[PlayerIndex].counter--;
			if	(m_Player[PlayerIndex].counter == 0)
			{
				if (m_Player[PlayerIndex].type != TYPE_COMPUTER)
				{
					//		m_Player[PlayerIndex].frame = 4;
					m_Player[PlayerIndex].status = STATUS_FREE;
				}
				else
				{
					if (m_Player[PlayerIndex].x%40 == 0 && m_Player[PlayerIndex].y%40 == 0)		//	����������
					{
						m_Player[PlayerIndex].frame = 4;
						m_Player[PlayerIndex].status = STATUS_FREE;
					}
					else																		//	��û�е�������
					{
						m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter�˴���ʾ��Ҫ������
						if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
						{
							m_Player[PlayerIndex].counter++;
						}
					}
				}
			}
		}
	}
}

void CSceneGAME::DoProperty()
{
	for (int PlayerIndex=0;PlayerIndex<PLAYER_NUM;PlayerIndex++)
	{
		if ( m_Player[PlayerIndex].type != TYPE_NULL
			&& (m_ScreenItem[(m_Player[PlayerIndex].y+20)/40][(m_Player[PlayerIndex].x+20)/40].type == _DJ_))
		{
			int	flag = true;
			switch(m_ScreenItem[(m_Player[PlayerIndex].y+20)/40][(m_Player[PlayerIndex].x+20)/40].tag)
			{
			case 0:				//	ը������
				if (m_Player[PlayerIndex].power<7)
				{
					m_Player[PlayerIndex].power++;
				}
				break;
			case 1:				//	ը������
				m_Player[PlayerIndex].bomb++;
				break;
			case 2:				//	�ƶ��ٶ�
				if (m_Player[PlayerIndex].status == STATUS_MOVE)
				{
					flag = false;
					break;
				}
				if (m_Player[PlayerIndex].speed<m_Player[PlayerIndex].step)
				{
					m_Player[PlayerIndex].speed++;
				}
				break;
			case 3:				//	������
				m_Player[PlayerIndex].cast = true;
				break;
			case 4:				//	������
				m_Player[PlayerIndex].kick = true;
				break;
			case 5:				//	������
				m_Player[PlayerIndex].pin = true;
				break;
			default:
				break;
			}
			if (flag)
			{
				memset((void*)&m_ScreenItem[(m_Player[PlayerIndex].y+20)/40][(m_Player[PlayerIndex].x+20)/40],0,sizeof(m_ScreenItem[(m_Player[PlayerIndex].y+20)/40][(m_Player[PlayerIndex].x+20)/40]));
				m_ScreenItem[(m_Player[PlayerIndex].y+20)/40][(m_Player[PlayerIndex].x+20)/40].tag = -1;
				sound.play(GET);
			}
		}
	}
}

bool CSceneGAME::FindSafety(int PlayerIndex)
{
	bool	flag = false;
	int		i;
	int		y = (m_Player[PlayerIndex].y+20)/40;
	int		x = (m_Player[PlayerIndex].x+20)/40;	


	flag = IsDangerous(x,y);

	if (flag)							//	��Ҫ��һ����ȫ�ĵط�
	{

		int		s = (m_Player[PlayerIndex].y+20)/40*15+(m_Player[PlayerIndex].x+20)/40;
		FindAddress(PlayerIndex,s);

		int min = INFINITYUSER;
		int	k = s;
		for (i=0;i<MaxVertexNum;i++)
		{
			if (m_Distance[i]<min && m_Distance[i]!=0 )
			{
				min = m_Distance[i];
				k = i;
			}
		}
		FindAddress(PlayerIndex,k);
	}

	return	flag;
}

bool CSceneGAME::FindPropertyForGet(int PlayerIndex)
{
	bool	flag = false;
	for (int i=0;i<13;i++)
	{
		for (int j=0;j<15;j++)
		{
			if (m_ScreenItem[i][j].type == _DJ_)		//	8��ʾ����
			{
				int	s = i*15 + j;

				//////////////////////////////////////////////////////////////////////////

				SetGraph(PlayerIndex);
				Dijkstra(m_Graph,m_Distance,m_Path,s);

				s = (m_Player[PlayerIndex].y+20)/40*15+(m_Player[PlayerIndex].x+20)/40;

				if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	�����·�����Ҳ���ͬһ��λ��
				{
					int pre = m_Path[s];
					if (IsDangerous(pre%15,pre/15))
					{
						return	false;
					}
					m_Player[PlayerIndex].status = STATUS_MOVE;						//	�����ƶ���־
					m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter�˴���ʾ��Ҫ������
					if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
					{
						m_Player[PlayerIndex].counter++;
					}

					if (pre == s-1)				//�����
					{
						m_Player[PlayerIndex].way = WAY_LEFT;
					}
					else	if (pre == s+1)		//���ұ�
					{
						m_Player[PlayerIndex].way = WAY_RIGHT;
					}
					else	if (pre == s-15)	//���ϱ�
					{
						m_Player[PlayerIndex].way = WAY_UP;
					}
					else						//���±�
					{
						m_Player[PlayerIndex].way = WAY_DOWN;
					}

					flag = true;
				}
				//////////////////////////////////////////////////////////////////////////

			}
		}
	}

	return	flag;
}

bool CSceneGAME::FindAdversaryForAttack(int PlayerIndex)
{
	if (m_Player[PlayerIndex].bomb == 0)
	{
		return	false;
	}

	bool	flag = false;
	m_PlayerLay[PlayerIndex] = false;
	int		s = (m_Player[0].y+20)/40*15+(m_Player[0].x+20)/40;

	//////////////////////////////////////////////////////////////////////////

	SetGraph(PlayerIndex);
	Dijkstra(m_Graph,m_Distance,m_Path,s);

	s = (m_Player[PlayerIndex].y+20)/40*15+(m_Player[PlayerIndex].x+20)/40;

	if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	�����·�����Ҳ���ͬһ��λ��
	{
		int pre = m_Path[s];
		if (IsDangerous(pre%15,pre/15))
		{
			return	false;
		}
		m_Player[PlayerIndex].status = STATUS_MOVE;						//	�����ƶ���־
		m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter�˴���ʾ��Ҫ������
		if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
		{
			m_Player[PlayerIndex].counter++;
		}

		if (pre == s-1)				//�����
		{
			m_Player[PlayerIndex].way = WAY_LEFT;
		}
		else	if (pre == s+1)		//���ұ�
		{
			m_Player[PlayerIndex].way = WAY_RIGHT;
		}
		else	if (pre == s-15)	//���ϱ�
		{
			m_Player[PlayerIndex].way = WAY_UP;
		}
		else						//���±�
		{
			m_Player[PlayerIndex].way = WAY_DOWN;
		}

		flag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	else	if(m_Distance[s] == 0)
	{
		m_PlayerLay[PlayerIndex] = true;
		m_LayX[PlayerIndex] = (m_Player[0].x+20)/40;			//	ը���ڷ�λ��x
		m_LayY[PlayerIndex] = (m_Player[0].y+20)/40;			//	ը���ڷ�λ��y
		flag = true;
	}

	return	flag;
}

bool CSceneGAME::FindLay(int PlayerIndex)				
{
	if (m_Player[PlayerIndex].bomb == 0)
	{
		return	false;
	}

	int i,j;
	bool	flag = false;
	m_PlayerLay[PlayerIndex] = false;
	int		s = (m_Player[PlayerIndex].y+20)/40*_LJ_W_+(m_Player[PlayerIndex].x+20)/40;
	FindAddress(PlayerIndex,s);


	int		Distance[MaxVertexNum];

	for (i=0;i<_LJ_H_;i++)
	{
		for (j=0;j<_LJ_W_;j++)
		{
			if (m_ScreenItem[i][j].type == _KG_ || m_ScreenItem[i][j].type == _DJ_)		//	�ö���Ϊ��
			{
				Distance[i*_LJ_W_+j] = GetDestroyNumber(PlayerIndex,j,i);
			}
			else
			{
				Distance[i*_LJ_W_+j] = 0;
			}
		}
	}

	int min = INFINITYUSER;
	int	k;
	for (i=0;i<MaxVertexNum;i++)
	{
		if (m_Distance[i]<min && Distance[i]>0)
		{
			min = m_Distance[i];
			k = i;
		}
	}

	if (min != INFINITYUSER)
	{
		////////////////////////////////////////////////////////////////////////

		SetGraph(PlayerIndex);
		Dijkstra(m_Graph,m_Distance,m_Path,k);

		s = (m_Player[PlayerIndex].y+20)/40*_LJ_W_+(m_Player[PlayerIndex].x+20)/40;

		if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	�����·�����Ҳ���ͬһ��λ��
		{
			int pre = m_Path[s];
			if (IsDangerous(pre%_LJ_W_,pre/_LJ_W_))
			{
				return	false;
			}
			m_Player[PlayerIndex].status = STATUS_MOVE;						//	�����ƶ���־
			m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter�˴���ʾ��Ҫ������
			if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
			{
				m_Player[PlayerIndex].counter++;
			}

			if (pre == s-1)				//�����
			{
				m_Player[PlayerIndex].way = WAY_LEFT;
			}
			else	if (pre == s+1)		//���ұ�
			{
				m_Player[PlayerIndex].way = WAY_RIGHT;
			}
			else	if (pre == s-_LJ_W_)	//���ϱ�
			{
				m_Player[PlayerIndex].way = WAY_UP;
			}
			else						//���±�
			{
				m_Player[PlayerIndex].way = WAY_DOWN;
			}

			flag = true;
		}
		////////////////////////////////////////////////////////////////////////
		else	if(m_Distance[k] == 0)
		{
			m_PlayerLay[PlayerIndex] = true;
			m_LayX[PlayerIndex] = k%_LJ_W_;			//	ը���ڷ�λ��x
			m_LayY[PlayerIndex] = k/_LJ_W_;			//	ը���ڷ�λ��y
		}
	}

	return	flag;
}

bool CSceneGAME::IsDangerous(int x,int y)
{
	bool	flag = false;

	int	i;
	for (i=0;i<_LJ_H_;i++)					//	���ж�y�������Ƿ���Σ��
	{
		if (m_ScreenItem[i][x].type == _PP_ && abs(y-i)<=m_ScreenItem[i][x].status && m_ScreenItem[i][x].counter<150)
		{
			flag = true;			//	��Σ��
			break;
		}
	}

	if (!flag)
	{
		for (i=0;i<_LJ_W_;i++)					//	���ж�x�������Ƿ���Σ��
		{
			if (m_ScreenItem[y][i].type == _PP_ && abs(x-i)<=m_ScreenItem[y][i].status && m_ScreenItem[y][i].counter<150)
			{
				flag = true;			//	��Σ��
				break;
			}
		}
	}

	return	flag;
}

bool CSceneGAME::FindAddress(int PlayerIndex,int s)
{
	bool flag = false;

	SetGraph(PlayerIndex);
	Dijkstra(m_Graph,m_Distance,m_Path,s);

	s = (m_Player[PlayerIndex].y+20)/40*_LJ_W_+(m_Player[PlayerIndex].x+20)/40;

	if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0)		//	�����·�����Ҳ���ͬһ��λ��
	{
		int pre = m_Path[s];
		//		if (IsDangerous(pre%15,pre/15))
		//		{
		//			return	false;
		//		}

		m_Player[PlayerIndex].status = STATUS_MOVE;						//	�����ƶ���־
		m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter�˴���ʾ��Ҫ������
		if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
		{
			m_Player[PlayerIndex].counter++;
		}

		if (pre == s-1)				//�����
		{
			m_Player[PlayerIndex].way = WAY_LEFT;
		}
		else	if (pre == s+1)		//���ұ�
		{
			m_Player[PlayerIndex].way = WAY_RIGHT;
		}
		else	if (pre == s-_LJ_W_)	//���ϱ�
		{
			m_Player[PlayerIndex].way = WAY_UP;
		}
		else						//���±�
		{
			m_Player[PlayerIndex].way = WAY_DOWN;
		}

		flag = true;
	}

	return	flag;
}

void CSceneGAME::SetGraph(int PlayerIndex)
{
	int		s = (m_Player[PlayerIndex].y+20)/40*_LJ_W_+(m_Player[PlayerIndex].x+20)/40;

	for (int i=0;i<MaxVertexNum;i++)
	{
		int	data1 = m_ScreenItem[i/_LJ_W_][i%_LJ_W_].type;
		if ( i == s)
		{
			data1 = 0;
		}
		for (int j=0;j<MaxVertexNum;j++)
		{
			int	data2 = m_ScreenItem[j/_LJ_W_][j%_LJ_W_].type;
			if ( j == s)
			{
				data2 = 0;
			}
			if (i/15 == j/_LJ_W_)			//	����������ͬһ����
			{
				if (j%_LJ_W_ == i%_LJ_W_-1 || j%_LJ_W_ == i%_LJ_W_+1)	//	�����ڵ���߻����ұ�
				{
					if ((data1 != 0 && data1 != 8)
						|| (data2 != 0 && data2 != 8))
					{
						m_Graph[i][j] = INFINITYUSER;
					}
					else
					{
						m_Graph[i][j] = 1;
					}
				}
				else
				{
					m_Graph[i][j] = INFINITYUSER;
				}
			}
			else	if (i%_LJ_W_ == j%_LJ_W_)	//	����������ͬһ����
			{
				if (j/_LJ_W_ == i/_LJ_W_-1 || j/_LJ_W_ == i/_LJ_W_+1)	//	�����ڵ��ϱ߻����±�
				{
					if ((data1 != 0 && data1 != 8)
						|| (data2 != 0 && data2 != 8))
					{
						m_Graph[i][j] = INFINITYUSER;
					}
					else
					{
						m_Graph[i][j] = 1;
					}
				}
				else
				{
					m_Graph[i][j] = INFINITYUSER;
				}
			}
			else
			{
				m_Graph[i][j] = INFINITYUSER;
			}
		}
	}
}

void CSceneGAME::Dijkstra(int G[MaxVertexNum][MaxVertexNum], int D[MaxVertexNum], int P[MaxVertexNum],int s)
{
	//	G�����������ڽӾ�������(i,j)�����ڣ�G[i][j] = INFINITYUSER
	//	D����̾���
	//	P�����·��
	//	��Դ�� 0 <= s <= MaxVertexNum-1

	bool	S[MaxVertexNum];				//	��㼯�ϣ�S[i]Ϊ���ʾ��㣬����Ϊ����
	int		i,j,k,min;
	for (i=0;i<MaxVertexNum;i++)
	{
		S[i] = false;						//	�ú�㼯Ϊ��
		D[i] = G[s][i];						//	�ó�ʼ�Ĺ��ƾ���
		if (D[i]<INFINITYUSER)					//	INFINITYUSER��ʾ�����
		{
			P[i] = s;						//	s��i��ǰ����˫�ף�
		}
		else
		{
			P[i] = -1;						//	i��ǰ����P[s]����ǰ������ΪG[s][s]��ΪINFINITYUSER
		}
	}

	S[s] = true;							//	��㼯��ʼʱֻ��Դ��
	D[s] = 0;								//	Դ�㵽Դ��ľ���Ϊ0

	for (i=0;i<MaxVertexNum-1;i++)			//	�����㼯
	{
		min = INFINITYUSER;
		for (j=0;j<MaxVertexNum;j++)		//	�ڵ�ǰ���㼯��ѡ���ƾ�����С�Ķ���k������s����Ķ���
		{
			if (!S[j] && D[j]<min)			//	j��������D[j]С�ڵ�ǰ��С�Ĺ��ƾ���
			{
				min = D[j];
				k = j;
			}
		}
		if (min == INFINITYUSER)					//	���㼯�����޹��ƾ���С��INFINITYUSER�Ķ���
		{
			return;
		}
		S[k] = true;						//	k�����㼯

		for (j=0;j<MaxVertexNum;j++)		//	����ʣ������Ĺ��ƾ���
		{
			if (!S[j] && (D[j] > D[k] + G[k][j]))
			{
				D[j] = D[k] + G[k][j];		//	�޸�����j�Ĺ��ƾ��룬ʹj��s����
				P[j] = k;					//	k��j��ǰ��
			}
		}
	}
}

int CSceneGAME::GetDestroyNumber(int PlayerIndex,int x,int y)
{
	int	counter = 0;
	int	i;
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	����ϱ�
	{
		if ((y-i)>=0 && (m_ScreenItem[y-i][x].type == _MK_ || m_ScreenItem[y-i][x].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	����±�
	{
		if ((y+i)<_LJ_H_ && (m_ScreenItem[y+i][x].type == _MK_ || m_ScreenItem[y+i][x].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	������
	{
		if ((x-i)>=0 && (m_ScreenItem[y][x-i].type == _MK_ || m_ScreenItem[y][x-i].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	����ұ�
	{
		if ((x+i)<_LJ_W_ && (m_ScreenItem[y][x+i].type == _MK_ || m_ScreenItem[y][x+i].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	return	counter;
}

void CSceneGAME::DoKill()
{
	for (int PlayerIndex=0;PlayerIndex<PLAYER_NUM;PlayerIndex++)
	{
		if ( m_Player[PlayerIndex].type == TYPE_COMPUTER
			&& (m_Player[PlayerIndex].x+20)/40 == (m_Player[0].x+20)/40
			&& (m_Player[PlayerIndex].y+20)/40 == (m_Player[0].y+20)/40)
		{
			if (m_Player[PlayerIndex].status !=STATUS_DIED )
			{
				if ( m_Player[0].status == STATUS_DIED && m_Player[PlayerIndex].counter > 60)
				{
					m_Player[0].counter = 60;
				}
			}
			else
			{
				if ( m_Player[0].status != STATUS_DIED && m_Player[PlayerIndex].counter > 60)
				{
					m_Player[PlayerIndex].counter = 60;
				}
			}
		}

		if ( m_Player[PlayerIndex].type == TYPE_COMPUTER
			&& (m_Player[PlayerIndex].x+20)/40 == (m_Player[1].x+20)/40
			&& (m_Player[PlayerIndex].y+20)/40 == (m_Player[1].y+20)/40)
		{
			if (m_Player[PlayerIndex].status !=STATUS_DIED )
			{
				if ( m_Player[1].status == STATUS_DIED && m_Player[PlayerIndex].counter > 60)
				{
					m_Player[1].counter = 60;
				}
			}
			else
			{
				if ( m_Player[1].status != STATUS_DIED && m_Player[PlayerIndex].counter > 60)
				{
					m_Player[PlayerIndex].counter = 60;
				}
			}
		}
	}
}

void CSceneGAME::SetBlast(int i,int j)
{
	int k;
	//	�ϱ�
	for (k=1;k<m_ScreenItem[i][j].status;k++)
	{
		if ( (i-k)<0 ||(m_ScreenItem[i-k][j].type == _MK_) || (m_ScreenItem[i-k][j].type == _XZ_) || (m_ScreenItem[i-k][j].type == _MZ_) || (m_ScreenItem[i-k][j].type == _QZ_))
		{
			break;							//	�����ľ�����ľ�����˳���ѭ��
		}
		if (m_ScreenItem[i-k][j].type == _PP_)
		{
			m_ScreenItem[i-k][j].counter = 1; 
			break;							//	����������򽫼�������Ϊ1�¸�ѭ���ͻᱬը
		}
		if (m_ScreenItem[i-k][j].type == _DJ_)
		{
			m_ScreenItem[i-k][j].tag = -1;
		}
		m_ScreenItem[i-k][j].type = _BZ_;
		m_ScreenItem[i-k][j].counter = 60; 
		m_ScreenItem[i-k][j].status = 1;
	}
	//	���ϱ�
	if ( (i-k)>=0 && (m_ScreenItem[i-k][j].type != _MZ_) && (m_ScreenItem[i-k][j].type != _QZ_))
	{
		if (m_ScreenItem[i-k][j].type == _PP_)
		{
			m_ScreenItem[i-k][j].counter = 1; 
		}
		else
		{
			if (m_ScreenItem[i-k][j].type == _DJ_)
			{
				m_ScreenItem[i-k][j].tag = -1;
			}
			m_ScreenItem[i-k][j].type = _BZ_;
			m_ScreenItem[i-k][j].counter = 60;
			m_ScreenItem[i-k][j].status = 2;
		}
	}
	//	�±�
	for (k=1;k<m_ScreenItem[i][j].status;k++)
	{
		if ( (i+k)>_LJ_H_ - 1 || (m_ScreenItem[i+k][j].type == _MK_) || (m_ScreenItem[i+k][j].type == _XZ_) || (m_ScreenItem[i+k][j].type == _MZ_) || (m_ScreenItem[i+k][j].type == _QZ_))
		{
			break;
		}
		if (m_ScreenItem[i+k][j].type == _PP_)
		{
			m_ScreenItem[i+k][j].counter = 1;
			break;
		}
		if (m_ScreenItem[i+k][j].type == _DJ_)
		{
			m_ScreenItem[i+k][j].tag = -1;
		}
		m_ScreenItem[i+k][j].type = _BZ_;
		m_ScreenItem[i+k][j].counter = 60;
		m_ScreenItem[i+k][j].status = 3;
	}
	//	���±�
	if ( (i+k)<=_LJ_H_ - 1 && (m_ScreenItem[i+k][j].type != _MZ_) && (m_ScreenItem[i+k][j].type != _QZ_))
	{
		if (m_ScreenItem[i+k][j].type == _PP_)
		{
			m_ScreenItem[i+k][j].counter = 1;
		}
		else
		{
			if (m_ScreenItem[i+k][j].type == _DJ_)
			{
				m_ScreenItem[i+k][j].tag = -1;
			}
			m_ScreenItem[i+k][j].type = _BZ_;
			m_ScreenItem[i+k][j].counter = 60;
			m_ScreenItem[i+k][j].status = 4;
		}
	}
	//	���
	for (k=1;k<m_ScreenItem[i][j].status;k++)
	{
		if ( (j-k)<0 || (m_ScreenItem[i][j-k].type == _MK_) || (m_ScreenItem[i][j-k].type == _XZ_) || (m_ScreenItem[i][j-k].type == _MZ_) || (m_ScreenItem[i][j-k].type == _QZ_))
		{
			break;
		}
		if (m_ScreenItem[i][j-k].type == _PP_)
		{
			m_ScreenItem[i][j-k].counter = 1;
			break;
		}
		if (m_ScreenItem[i][j-k].type == _DJ_)
		{
			m_ScreenItem[i][j-k].tag = -1;
		}
		m_ScreenItem[i][j-k].type = _BZ_;
		m_ScreenItem[i][j-k].counter = 60;
		m_ScreenItem[i][j-k].status = 5;
	}
	//	�����
	if ( (j-k)>=0 && (m_ScreenItem[i][j-k].type != _MZ_) && (m_ScreenItem[i][j-k].type != _QZ_))
	{
		if (m_ScreenItem[i][j-k].type == _PP_)
		{
			m_ScreenItem[i][j-k].counter = 1;
		}
		else
		{
			if (m_ScreenItem[i][j-k].type == _DJ_)
			{
				m_ScreenItem[i][j-k].tag = -1;
			}
			m_ScreenItem[i][j-k].type = _BZ_;
			m_ScreenItem[i][j-k].counter = 60;
			m_ScreenItem[i][j-k].status = 6;
		}
	}
	//	�ұ�
	for (k=1;k<m_ScreenItem[i][j].status;k++)
	{
		if ( (j+k)>_LJ_W_ - 1 || (m_ScreenItem[i][j+k].type == _MK_) || (m_ScreenItem[i][j+k].type == _XZ_) || (m_ScreenItem[i][j+k].type == _MZ_) || (m_ScreenItem[i][j+k].type == _QZ_))
		{
			break;
		}
		if (m_ScreenItem[i][j+k].type == _PP_)
		{
			m_ScreenItem[i][j+k].counter = 1;
			break;
		}
		if (m_ScreenItem[i][j+k].type == _DJ_)
		{
			m_ScreenItem[i][j+k].tag = -1;
		}
		m_ScreenItem[i][j+k].type = _BZ_;
		m_ScreenItem[i][j+k].counter = 60;
		m_ScreenItem[i][j+k].status = 7;
	}
	//	���ұ�
	if ( (j+k)<=_LJ_W_ - 1 && (m_ScreenItem[i][j+k].type != _MZ_) && (m_ScreenItem[i][j+k].type != _QZ_))
	{
		if (m_ScreenItem[i][j+k].type == _PP_)
		{
			m_ScreenItem[i][j+k].counter = 1;
		}
		else
		{
			if (m_ScreenItem[i][j+k].type == _DJ_)
			{
				m_ScreenItem[i][j+k].tag = -1;
			}
			m_ScreenItem[i][j+k].type = _BZ_;
			m_ScreenItem[i][j+k].counter = 60;
			m_ScreenItem[i][j+k].status = 8;
		}
	}
	//	���Ĵ�
	m_ScreenItem[i][j].type = _BZ_;
	m_ScreenItem[i][j].counter = 60;
	m_ScreenItem[i][j].status = 0;

	m_Player[m_ScreenItem[i][j].who].bomb++;						//	ը�����ָ�

}

void CSceneGAME::DoKick()
{
	RECT	rect;
	for (list<MOVEBOMB>::iterator iter = m_bombKick.begin(); iter != m_bombKick.end(); iter++)
	{

		//	�ж��Ƿ񵽴�Ŀ��㣬��������������ɾ����Ԫ�أ������ݼ��뵽������Ϣ��
		bool	deleteflag = false;
		//	�߳���������(ǰ�����赲��ֹͣ�˶�)
		switch(iter->way)
		{
		case WAY_UP:
			if (iter->y%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->y == 0 || m_ScreenItem[iter->y/40-1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40-1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_DOWN:
			if (iter->y%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->y == 12*40 || m_ScreenItem[iter->y/40+1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40+1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_LEFT:
			if (iter->x%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->x == 0 || m_ScreenItem[iter->y/40][iter->x/40-1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40-1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_RIGHT:
			if (iter->x%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->x == 14*40 || m_ScreenItem[iter->y/40][iter->x/40+1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40+1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		default:
			break;
		}
		if (deleteflag)
		{
			m_bombKick.erase(iter);
			break;
		}

		//	�߳���������(ÿ���ƶ�5������)
		switch(iter->way)
		{
		case WAY_UP:
			iter->y -= 5;
			break;
		case WAY_DOWN:
			iter->y += 5 ;
			break;
		case WAY_LEFT:
			iter->x -= 5;
			break;
		case WAY_RIGHT:
			iter->x += 5;
			break;
		default:
			break;
		}
		rect.left = 480;
		rect.top = 304;
		rect.right = 480+44;
		rect.bottom = 304+44;
		PGF->GetGO()->tDrawBmp(iter->x+19-2, iter->y+40-2, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

	}
}

void CSceneGAME::DoCast()
{
	RECT	rect;
	for (list<MOVEBOMB>::iterator iter = m_bombCast.begin(); iter != m_bombCast.end(); iter++)
	{

		//	�ж��Ƿ񵽴�Ŀ��㣬��������������ɾ����Ԫ�أ������ݼ��뵽������Ϣ��
		bool	deleteflag = false;
		//	�ӳ���������(����Ŀ���֮����Ŀ������޶�������ֹͣ���������¼���Ŀ���)
		switch(iter->way)
		{
		case WAY_UP:
			if (iter->y%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->y == 0 || m_ScreenItem[iter->y/40-1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40-1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_DOWN:
			if (iter->y%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->y == 12*40 || m_ScreenItem[iter->y/40+1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40+1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_LEFT:
			if (iter->x%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->x == 0 || m_ScreenItem[iter->y/40][iter->x/40-1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40-1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		case WAY_RIGHT:
			if (iter->x%40 == 0)
			{
				//	�˴����ǰ��������ң�������ֹͣǰ��

				//	�˴����ǰ�������赲�������ֹͣǰ��
				if (iter->x == 14*40 || m_ScreenItem[iter->y/40][iter->x/40+1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40+1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	����ɾ����־
				}
			}
			break;
		default:
			break;
		}

		if (deleteflag)
		{
			m_bombCast.erase(iter);
			break;
		}

		//	�ӳ���������(ÿ���ƶ�1������)
		switch(iter->way)
		{
		case WAY_UP:
			iter->y -= 2;
			break;
		case WAY_DOWN:
			iter->y += 2 ;
			break;
		case WAY_LEFT:
			iter->x -= 2;
			break;
		case WAY_RIGHT:
			iter->x += 2;
			break;
		default:
			break;
		}
		//	������������Ʈ���Ĳ���
		if (iter->addorsub)
		{
			iter->offset++;
			if (iter->offset == 30)
			{
				iter->addorsub = !iter->addorsub;
			}
		}
		else
		{
			iter->offset--;
			if (iter->offset == 0)
			{
				iter->addorsub = !iter->addorsub;
			}

		}

		rect.left = 480;
		rect.top = 304;
		rect.right = 480+44;
		rect.bottom = 304+44;
		PGF->GetGO()->tDrawBmp(iter->x+19-2, iter->y+40-2-iter->offset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));


	}
}

void CSceneGAME::XR()
{
	RECT	rect;
	int		i,j;

	//	�������
	PGF->GetGO()->tDrawBmp(0, 0, 800, 600,0,0, "bmp\\pic.bmp",RGB(255,0,255));

	//	����
	rect.top = 160;
	rect.bottom = 200;
	for(i=0;i<_LJ_H_;i++)
	{
		for(j=0;j<_LJ_W_;j++)
		{
			rect.left = 40+(i+j)%2*40;
			rect.right = 80+(i+j)%2*40;
			PGF->GetGO()->tDrawBmp(j*40+19, i*40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

		}
	}

	//	��ʾ����
	for(i=0;i<_LJ_H_;i++)
	{
		for(j=_LJ_W_ - 1;j>=0;j--)
		{
			switch(m_ScreenItem[i][j].type)
			{
			case _MK_:					//	ľ��
				rect.left = 120;
				rect.top = 120;
				rect.right = 160;
				rect.bottom = 200;
				PGF->GetGO()->tDrawBmp(j*40+19, i*40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				if (j!=0)			//	�����Ӱ
				{
					rect.left = 480;
					rect.top = 348;
					rect.right = 480+5;
					rect.bottom = 348+39;
					PGF->GetGO()->tDrawBmp(j*40+19-5, i*40+40+1, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				if (i!=12)			//	�±���Ӱ
				{
					rect.top = 387;
					rect.right = 480+44;
					rect.bottom = 387+5;
					if (j == 0)
					{
						rect.left = 480+5;
						PGF->GetGO()->tDrawBmp(j*40+19, i*40+40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

					}
					else
					{
						rect.left = 480;
						PGF->GetGO()->tDrawBmp(j*40+19-5, i*40+40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

					}
				}
				break;
			case _XZ_:					//	����

				//	�ж������Ƿ����˶��������޸�������ƫ����
				if (m_ScreenItem[i][j].counter != 0)
				{
					m_ScreenItem[i][j].counter--;
					switch(m_ScreenItem[i][j].way)
					{
					case WAY_UP:
						m_ScreenItem[i][j].yoffset = -40+m_ScreenItem[i][j].counter;
						if (m_ScreenItem[i][j].counter == 0)
						{
							m_ScreenItem[i-1][j].type = m_ScreenItem[i][j].type;
							m_ScreenItem[i-1][j].status = m_ScreenItem[i][j].status;
							m_ScreenItem[i-1][j].counter = m_ScreenItem[i][j].counter;
							m_ScreenItem[i-1][j].xoffset = m_ScreenItem[i-1][j].yoffset = 0;
							m_ScreenItem[i][j].type = _KG_;
						}
						break;
					case WAY_DOWN:
						m_ScreenItem[i][j].yoffset = 40-m_ScreenItem[i][j].counter;
						if (m_ScreenItem[i][j].counter == 0)
						{
							m_ScreenItem[i+1][j].type = m_ScreenItem[i][j].type;
							m_ScreenItem[i+1][j].status = m_ScreenItem[i][j].status;
							m_ScreenItem[i+1][j].counter = m_ScreenItem[i][j].counter;
							m_ScreenItem[i+1][j].xoffset = m_ScreenItem[i+1][j].yoffset = 0;
							m_ScreenItem[i][j].type = _KG_;
						}
						break;
					case WAY_LEFT:
						m_ScreenItem[i][j].xoffset = -40+m_ScreenItem[i][j].counter;
						if (m_ScreenItem[i][j].counter == 0)
						{
							m_ScreenItem[i][j-1].type = m_ScreenItem[i][j].type;
							m_ScreenItem[i][j-1].status = m_ScreenItem[i][j].status;
							m_ScreenItem[i][j-1].counter = m_ScreenItem[i][j].counter;
							m_ScreenItem[i][j-1].xoffset = m_ScreenItem[i][j-1].yoffset = 0;
							m_ScreenItem[i][j].type = _KG_;
						}
						break;
					case WAY_RIGHT:
						m_ScreenItem[i][j].xoffset = 40-m_ScreenItem[i][j].counter;
						if (m_ScreenItem[i][j].counter == 0)
						{
							m_ScreenItem[i][j+1].type = m_ScreenItem[i][j].type;
							m_ScreenItem[i][j+1].status = m_ScreenItem[i][j].status;
							m_ScreenItem[i][j+1].counter = m_ScreenItem[i][j].counter;
							m_ScreenItem[i][j+1].xoffset = m_ScreenItem[i][j+1].yoffset = 0;
							m_ScreenItem[i][j].type = _KG_;
						}
						break;
					default:
						break;
					}
				}
				rect.left = 160;
				rect.top = 120;
				rect.right = 200;
				rect.bottom = 200;
				PGF->GetGO()->tDrawBmp(j*40+19+m_ScreenItem[i][j].xoffset, i*40+m_ScreenItem[i][j].yoffset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				if (j!=0)			//	�����Ӱ
				{
					rect.left = 480;
					rect.top = 348;
					rect.right = 480+5;
					rect.bottom = 348+39;
					PGF->GetGO()->tDrawBmp(j*40+19-5+m_ScreenItem[i][j].xoffset, i*40+40+1+m_ScreenItem[i][j].yoffset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				if (i!=12)			//	�±���Ӱ
				{
					rect.top = 387;
					rect.right = 480+44;
					rect.bottom = 387+5;
					if (j == 0)
					{
						rect.left = 480+5;
						PGF->GetGO()->tDrawBmp(j*40+19+m_ScreenItem[i][j].xoffset, i*40+40+40+m_ScreenItem[i][j].yoffset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

					}
					else
					{
						rect.left = 480;
						PGF->GetGO()->tDrawBmp(j*40+19-5+m_ScreenItem[i][j].xoffset, i*40+40+40+m_ScreenItem[i][j].yoffset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

					}
				}
				break;
			case _MZ_:					//	ľ׮
				rect.left = 200;
				rect.top = 120;
				rect.right = 240;
				rect.bottom = 200;
				PGF->GetGO()->tDrawBmp(j*40+19, i*40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				break;
			case _QZ_:					//	������
				if (m_ScreenItem[i][j].status == 0)
				{
					rect.left = 240;
					rect.top = 40;
					rect.right = 440;
					rect.bottom = 240;
					PGF->GetGO()->tDrawBmp(5*40+19, 2*40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				break;
			case _PP_:					//	���ݣ���ֹ�ģ�������ƶ��Ķ��ԣ�
				rect.left = 480+m_ScreenItem[i][j].counter/30%3*44;
				rect.top = 304;
				rect.right = 480+44+m_ScreenItem[i][j].counter/30%3*44;
				rect.bottom = 304+44;
				PGF->GetGO()->tDrawBmp(j*40+19-2, i*40+40-4, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				m_ScreenItem[i][j].counter--;
				if (m_ScreenItem[i][j].counter <= 0)		//	���ݱ�ը
				{
					SetBlast(i,j);
					sound.play(DETONATION);		//	���ű�ը��Ч
				}
				break;
			case _DJ_:					//	����
				if (m_ScreenItem[i][j].counter % 10 == 0)
				{
					if (m_ScreenItem[i][j].status == 0)			//	��
					{
						m_ScreenItem[i][j].yoffset++;
						if (m_ScreenItem[i][j].yoffset == 3)
						{
							m_ScreenItem[i][j].status = -1;
						}
					}
					else										//	��
					{
						m_ScreenItem[i][j].yoffset--;
						if (m_ScreenItem[i][j].yoffset == -3)
						{
							m_ScreenItem[i][j].status = 0;
						}
					}
				}

				rect.left = 120;
				rect.top = 40;
				rect.right = 120+20;
				rect.bottom = 40+20;
				if (m_ScreenItem[i][j].yoffset>0)
				{
					rect.left = 120+20;
					rect.right = 120+20+20;
				}
				PGF->GetGO()->tDrawBmp(j*40+19+10, i*40+40+20, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				//	tag��ʾ���ߵ�����
				rect.left = 480 + m_ScreenItem[i][j].counter/10%3*44;
				rect.top = 40+m_ScreenItem[i][j].tag*44;
				rect.right = 480 + 44 + m_ScreenItem[i][j].counter/10%3*44;
				rect.bottom = 40+44+m_ScreenItem[i][j].tag*44;
				PGF->GetGO()->tDrawBmp(j*40+19-2, i*40+40-24, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				m_ScreenItem[i][j].counter++;
				if (m_ScreenItem[i][j].counter == 1000)
				{
					m_ScreenItem[i][j].counter = 0;
				}
				break;
			case _BZ_:											//	��ըЧ��
				{
					//	status�ڴ˴���ʾ��ըЧ���ı��
					/*
					2
					1
					...
					1
					6 5 ... 5 0 7 ... 7 8
					3
					...
					3
					4


					*/
					m_ScreenItem[i][j].counter--;
					if (m_ScreenItem[i][j].counter == 0)		//	��ը����
					{
						if (m_ScreenItem[i][j].tag != -1)
						{
							m_ScreenItem[i][j].type = _DJ_;		//	ը�����ĵ���
							sound.play(APPEAR);		//	���ŵ��߳�����Ч
						}
						else
						{
							m_ScreenItem[i][j].type = _KG_;
						}
					}

					int temp = m_ScreenItem[i][j].counter/6;
					if (temp>4)	temp=4;

					rect.left = 40+(4-temp)*40;
					rect.top = 200+m_ScreenItem[i][j].status*40;
					rect.right = 40+40+(4-temp)*40;
					rect.bottom = 200+40+m_ScreenItem[i][j].status*40;
					PGF->GetGO()->tDrawBmp(j*40+19, i*40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				break;
			default:
				break;
			}
		}

		//	��ʾ���	DrawPlayer
		for (int PlayerIndex=0;PlayerIndex<PLAYER_NUM;PlayerIndex++)
		{
			if ((m_Player[PlayerIndex].y+20)/40 == i && m_Player[PlayerIndex].type != TYPE_NULL)
			{
				if (m_Player[PlayerIndex].status == STATUS_DIED)				//	��ը��
				{
					rect.left = 280;
					rect.top = 280;
					rect.right = 280+60;
					rect.bottom = 280+60;
					if (m_Player[PlayerIndex].counter == 60)
					{
						sound.play(WIN);
						sound.play(LOST);
					}
					if (m_Player[PlayerIndex].counter < 60)
					{
						rect.top += 60;
						rect.bottom += 60;
					}
					if (m_Player[PlayerIndex].counter != 0)
					{
						m_Player[PlayerIndex].counter--;
						PGF->GetGO()->tDrawBmp(m_Player[PlayerIndex].x+19-10, m_Player[PlayerIndex].y+40-20, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

					}
				}
				else
				{
					rect.left = m_Player[PlayerIndex].way*52*5+m_Player[PlayerIndex].frame*52;
					rect.top = m_Player[PlayerIndex].image*62;
					rect.right = m_Player[PlayerIndex].way*52*5+52+m_Player[PlayerIndex].frame*52;
					rect.bottom = m_Player[PlayerIndex].image*62+62;
					PGF->GetGO()->tDrawBmp(m_Player[PlayerIndex].x+19-6, m_Player[PlayerIndex].y+40-20, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\player.bmp",RGB(255,0,255));

				}
			}
		}
	}

	char	str[256];
	if (m_bHelp)
	{
		sprintf_s(str,"FPS: %f ����1��Ϣ x: %d y: %d way: %d speed: %d",m_fps,m_Player[0].x,m_Player[0].y,m_Player[0].way,m_Player[0].speed);
		PGF->GetGO()->DrawText(str,19, 40, RGB(255,255,0));
		if(DS)
		{
			sprintf_s(str,"FPS: %f ����2��Ϣ x: %d y: %d way: %d speed: %d",m_fps,m_Player[1].x,m_Player[1].y,m_Player[1].way,m_Player[1].speed);
			PGF->GetGO()->DrawText(str,19, 60, RGB(255,255,0));

		}
		else
		{
			sprintf_s(str,"FPS: %f ������Ϣ x: %d y: %d way: %d speed: %d",m_fps,m_Player[1].x,m_Player[1].y,m_Player[1].way,m_Player[1].speed);
			PGF->GetGO()->DrawText(str,19, 60, RGB(255,255,0));

		}
		sprintf_s(str,"F1 ����");
		PGF->GetGO()->DrawText(str,19, 80, RGB(255,255,0));

		sprintf_s(str,"F2 ����");
		PGF->GetGO()->DrawText(str,19, 100, RGB(255,255,0));

		sprintf_s(str,"F3 ����");
		PGF->GetGO()->DrawText(str,19, 120, RGB(255,255,0));

		sprintf_s(str,"1 ����");
		PGF->GetGO()->DrawText(str,19, 140, RGB(255,255,0));

		sprintf_s(str,"2 ����");
		PGF->GetGO()->DrawText(str,19, 160, RGB(255,255,0));

		sprintf_s(str,"3 ����");
		PGF->GetGO()->DrawText(str,19, 180, RGB(255,255,0));

	}


	//	��ʾСͼ��
	for (i=0;i<PLAYER_NUM;i++)
	{
		if (m_Player[i].type != TYPE_NULL)
		{
			rect.left = 4*52;
			rect.top = m_Player[i].image*62;
			rect.right = 4*52+52;
			rect.bottom = m_Player[i].image*62+38;
			PGF->GetGO()->tDrawBmp(655, 60+i*44, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\player.bmp",RGB(255,0,255));


			if (m_Player[i].type == TYPE_USER)
				PGF->GetGO()->DrawText("���һ",705, 87+i*44, RGB(255,255,0));
			else
			{
				if(DS)
					PGF->GetGO()->DrawText("��Ҷ�",705, 87+i*44, RGB(255,255,0));
				else
					PGF->GetGO()->DrawText("�������",705, 87+i*44, RGB(255,255,0));
			}
		}
	}
}

void    CSceneGAME:: DoInput1()
{
	if(PGF->GetGI()->Get(_GI_K_1) == _KS_UC)
	{
		if(jy)
		{
			sound.SetVolume(0.0f);
			sound.SetIsMute(true);
			jy = false;
		}
		else
		{	
			sound.SetVolume(_YL_);
			sound.SetIsMute(false);
			jy = true;
		}
	}

	if(PGF->GetGI()->Get(_GI_K_2) == _KS_UC && jy)
	{
		if(yl < VOLUME_MAX_VALUE)
		{
			yl += VOLUME_CHANGE_ONCE_VALUE;
			sound.SetVolume(yl);
		}
	}
	if(PGF->GetGI()->Get(_GI_K_3) == _KS_UC && jy)
	{
		if(yl > VOLUME_MIN_VALUE)
		{
			yl -= VOLUME_CHANGE_ONCE_VALUE;
			sound.SetVolume(yl);
		}
	}

	if (PGF->GetGI()->Get(_GI_K_F01) == _KS_UC)
	{
		m_bHelp = !m_bHelp;
	}

	if (PGF->GetGI()->Get(_GI_K_F02) == _KS_UC)
	{
		sound.play(START);
		ResetMapInfor();
		return;
	}

	if (PGF->GetGI()->Get(_GI_K_F03) == _KS_UC)
	{
		PGF->SetCurScene("��ʼ����");
		return;
	}
	if(PGF->GetGI()->Get(_GI_K_L_CTRL) == _KS_UC)
	{
		m_Player_cz = _PLAY_CTRL_;
		m_id = 0;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_W) == _KS_DH || PGF->GetGI()->Get(_GI_K_W) == _KS_UC)
	{
		m_Player_cz = _PLAY_UP_;
		m_id = 0;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_S) == _KS_DH || PGF->GetGI()->Get(_GI_K_S) == _KS_UC)
	{
		m_Player_cz = _PLAY_DOWN_;
		m_id = 0;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_A) == _KS_DH || PGF->GetGI()->Get(_GI_K_A) == _KS_UC)
	{
		m_Player_cz = _PLAY_LEFT_;
		m_id = 0;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_D) == _KS_DH || PGF->GetGI()->Get(_GI_K_D) == _KS_UC)
	{
		m_Player_cz = _PLAY_RIGHT_;
		m_id = 0;
		DoInput();	
	}

	if(DS)
	{
		DoInput2();	
	}


	//	����5���������ڴ��������ҵ��˹�����(ÿ�ƶ�һ�������ж�һ�Σ���Ϊ�����ϵ���Ϣ��ʱ�ڸı�)
	else
	{
		for (int i=0;i<PLAYER_NUM;i++)
		{
			if (m_Player[i].type == TYPE_COMPUTER && m_Player[i].status == STATUS_FREE)
			{
				//	�Ѿ��������������ˣ�m_MoveCount[i]����֮ǰ���ã�����һ��֮���1

				if (!FindSafety(i))							//	����һ����ȫ��λ�ã��ҵ���ȥ���
				{
					//	Ŀǰ��վλ���ǰ�ȫ��
					//				if (!FindTeammateForRescue(i))			//	������ҪӪ�ȵĶ��ѵ�λ��
					{
						if (!FindPropertyForGet(i))			//	�鿴���ߵ�λ�ã����ҵ���ȥ��
						{
							if (!FindAdversaryForAttack(i))	//	���Ҷ��ֵ�λ�ã��ҵ���ȥ����
							{
								if (!FindLay(i))				//	����һ������ը����λ��,�Ա�ը��һЩ����
								{

								}
							}
						}
					}
				}

				//�ж��Ƿ�Ҫ��ը��
				if (m_PlayerLay[i])
				{
					if (m_Player[i].x == m_LayX[i]*40 && m_Player[i].y == m_LayY[i]*40)
					{
						if ( (m_Player[i].bomb != 0) && 
							(m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].type != _PP_))
						{
							m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].type = _PP_;
							m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].counter = 200;
							m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].status = m_Player[i].power;
							m_ScreenItem[(m_Player[i].y+20)/40][(m_Player[i].x+20)/40].who = i;
							m_Player[i].bomb--;
							sound.play(LAY);
						}
						m_PlayerLay[i] = false;
					}
				}
			}
		}
	}
}

void  CSceneGAME::DoInput2()	 
{
	if(PGF->GetGI()->Get(_GI_K_R_CTRL) == _KS_UC)
	{
		m_Player_cz = _PLAY_CTRL_;
		m_id = 1;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_UP) == _KS_DH || PGF->GetGI()->Get(_GI_K_UP) == _KS_UC)
	{
		m_Player_cz = _PLAY_UP_;
		m_id = 1;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_DOWN) == _KS_DH || PGF->GetGI()->Get(_GI_K_DOWN) == _KS_UC)
	{
		m_Player_cz = _PLAY_DOWN_;
		m_id = 1;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_LEFT) == _KS_DH || PGF->GetGI()->Get(_GI_K_LEFT) == _KS_UC)
	{
		m_Player_cz = _PLAY_LEFT_;
		m_id = 1;
		DoInput();	
	}

	if(PGF->GetGI()->Get(_GI_K_RIGHT) == _KS_DH || PGF->GetGI()->Get(_GI_K_RIGHT) == _KS_UC)
	{
		m_Player_cz = _PLAY_RIGHT_;
		m_id = 1;
		DoInput();	
	}
}