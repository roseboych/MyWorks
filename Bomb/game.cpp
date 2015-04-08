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
		PGF->GetGO()->SetTitleText("泡泡堂---玩家对战");
	else
		PGF->GetGO()->SetTitleText("泡泡堂---人机对战");
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
	LoadUI(new CButton6(650,560,780,590,"bmp\\kongjian2.bmp","bmp\\kongjian1.bmp"), "按钮1");
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

	//	1.逻辑处理部分
	if (m_GameState == GAME_FREE || m_GameState == GAME_START)
	{
		DoInput1();					//	处理按键
		DoMove();					//	处理角色移动
		DoProperty();				//	处理道具
		DoKill();
	}


	for (i=0;i<PLAYER_NUM;i++)		//	判断是否被炸
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

	//	判断游戏结果
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
	//	处理踢出来的泡泡
	DoKick();

	//	处理扔出来的泡泡
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
			m_ScreenItem[i][j].tag = -1;				//	表示无道具
			if (rand()%100>_XZ_JL_)
			{
				m_ScreenItem[i][j].type = _KG_;			//	0表示空
			}
			else
			{
				if (rand()%100<_DJ_JL_)						//	30% 的几率
				{
					m_ScreenItem[i][j].tag = rand()%_DJ_ZL_;
				}

				if (rand()%100<_MK_JL_)
				{
					m_ScreenItem[i][j].type = _MK_;		//	1表示木块
				}
				else
				{
					m_ScreenItem[i][j].type = _XZ_;		//	2表示箱子
				}
			}
		}
	}

	m_ScreenItem[1][1].type = _MZ_;						//	3表示木桩
	m_ScreenItem[1][13].type = _MZ_;
	m_ScreenItem[11][1].type = _MZ_;
	m_ScreenItem[11][13].type = _MZ_;

	m_ScreenItem[6][6].type = _QZ_;						//	4,5,6表示海盗旗
	m_ScreenItem[6][6].status = 0;
	m_ScreenItem[6][7].type = _QZ_;
	m_ScreenItem[6][7].status = 1;
	m_ScreenItem[6][8].type = _QZ_;
	m_ScreenItem[6][8].status = 2;

	m_ScreenItem[0][1].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[0][1].tag = -1;
	m_ScreenItem[0][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[0][2].tag = -1;
	m_ScreenItem[1][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[1][2].tag = -1;
	m_ScreenItem[2][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[2][2].tag = -1;
	  
	m_ScreenItem[0][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[0][12].tag = -1;
	m_ScreenItem[1][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[1][12].tag = -1;
	m_ScreenItem[2][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[2][12].tag = -1;
	m_ScreenItem[0][13].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[0][13].tag = -1;

	m_ScreenItem[10][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[10][2].tag = -1;
	m_ScreenItem[11][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[11][2].tag = -1;
	m_ScreenItem[12][2].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[12][2].tag = -1;
	m_ScreenItem[12][1].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[12][1].tag = -1;

	m_ScreenItem[10][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[10][12].tag = -1;
	m_ScreenItem[11][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[11][12].tag = -1;
	m_ScreenItem[12][12].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[12][12].tag = -1;
	m_ScreenItem[10][13].type = _KG_;						//	预留4处空的位置
	m_ScreenItem[10][13].tag = -1;

	for (int i=0;i<PLAYER_NUM;i++)
	{
		m_Player[i].type = TYPE_NULL;
		m_PlayerLay[i] = false;
	}

	//	主角信息
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

	pos =(pos+2)%4;						//	取对角位置
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
		if (m_Player[m_id].y % 40 == 0)	//	判断阻挡
		{
			done = CAN_BLOCK;
			//	正前方为空
			if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _BZ_ ))
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	直接往前走
				}
				else	//	右上方有东西
				{
					if (m_Player[m_id].x%40 == 0)
					{
						done = CAN_MOVE;		//	直接往前走
					}
					else if (m_Player[m_id].x%40 <= 30)
					{
						m_Player[m_id].way = WAY_LEFT;
						done = CAN_MOVE;		//	向左边偏移
					}
					else if (m_Player[m_id].x%40 > 30)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	正前方有东西
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].x%40 == 0)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	正上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	正上方是泡泡，踢
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].x%40 >= 10)
					{
						m_Player[m_id].way = WAY_RIGHT;
						done = CAN_MOVE;		//	向右边偏移
					}
					else if (m_Player[m_id].x%40 < 10)
					{
						if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	正上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	正上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
				else	//	右上方有东西
				{
					//	此处较麻烦分4种情况:	1.左边可以移动，右边可以移动	如果x%40<20则左移，否则右移
					//							2.左边可以移动，右边不可移动	如果x%40<=30则左移
					//							3.左边不可移动，右边可以移动	如果x%40>=10则右移
					//							4.左边不可移动，右边不可移动	不能移动
					if (((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	正上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	正上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 < 20)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	向左边偏移
						}
						else
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	向右边偏移
						}
					}

					if (((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& !((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	正上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	正上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 <= 30)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	向左边偏移
						}
					}

					if ( !((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y-40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 >= 10)
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	向右边偏移
						}
					}
				}
			}
		}
		test = done;
		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	设置移动标志
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter此处表示需要多少桢
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
				//	设置推箱子标志
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
				//	设置踢泡泡标志
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
		if (m_Player[m_id].y % 40 == 0)	//	判断阻挡
		{
			done = CAN_BLOCK;
			//	正前方为空
			if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _BZ_ ))
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	直接往前走
				}
				else	//	右上方有东西
				{
					if (m_Player[m_id].x%40 == 0)
					{
						done = CAN_MOVE;		//	直接往前走
					}
					else if (m_Player[m_id].x%40 <= 30)
					{
						m_Player[m_id].way = WAY_LEFT;
						done = CAN_MOVE;		//	向左边偏移
					}
					else if (m_Player[m_id].x%40 > 30)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type)
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_)
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	正前方有东西
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].x%40 == 0)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	正上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	正上方是泡泡，踢
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].x%40 >= 10)
					{
						m_Player[m_id].way = WAY_RIGHT;
						done = CAN_MOVE;		//	向右边偏移
					}
					else if (m_Player[m_id].x%40 < 10)
					{
						if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
						{
							//	正上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
						{
							//	正上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
				else	//	右上方有东西
				{
					//	此处较麻烦分4种情况:	1.左边可以移动，右边可以移动	如果x%40<20则左移，否则右移
					//							2.左边可以移动，右边不可移动	如果x%40<=30则左移
					//							3.左边不可移动，右边可以移动	如果x%40>=10则右移
					//							4.左边不可移动，右边不可移动	不能移动
					if (((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 < 20)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;	//	向左边偏移
						}
						else
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;	//	向右边偏移
						}
					}

					if (((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& !((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 == 0)
						{
							if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].x%40 <= 30)
						{
							m_Player[m_id].way = WAY_LEFT;
							done = CAN_MOVE;		//	向左边偏移
						}
					}

					if ( !((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40].type == _PP_ ))
						&& ((m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[(m_Player[m_id].y+40)/40][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].x%40 >= 10)
						{
							m_Player[m_id].way = WAY_RIGHT;
							done = CAN_MOVE;		//	向右边偏移
						}
					}
				}
			}
		}
		test = done;
		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	设置移动标志
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter此处表示需要多少桢
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
				//	设置推箱子标志
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
				//	设置踢泡泡标志
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
		if (m_Player[m_id].x % 40 == 0)	//	判断阻挡
		{
			done = CAN_BLOCK;
			//	正前方为空
			if ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _BZ_ ))
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	直接往前走
				}
				else	//	右上方有东西
				{
					if (m_Player[m_id].y%40 == 0)
					{
						done = CAN_MOVE;		//	直接往前走
					}
					else if (m_Player[m_id].y%40 <= 30)
					{
						m_Player[m_id].way = WAY_UP;
						done = CAN_MOVE;		//	向左边偏移
					}
					else if (m_Player[m_id].y%40 > 30)
					{
						if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	正前方有东西
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _BZ_ ))
				{

					if (m_Player[m_id].y%40 == 0)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	右上方是箱子，推
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
						done = CAN_MOVE;		//	向右边偏移
					}
					else if ( m_Player[m_id].y % 40 < 10)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
				else	//	右上方有东西
				{
					//	此处较麻烦分4种情况:	1.左边可以移动，右边可以移动	如果x%40<20则左移，否则右移
					//							2.左边可以移动，右边不可移动	如果x%40<=30则左移
					//							3.左边不可移动，右边可以移动	如果x%40>=10则右移
					//							4.左边不可移动，右边不可移动	不能移动
					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 < 20)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	向左边偏移
						}
						else
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	向右边偏移
						}
					}

					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& !((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 <= 30)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	向左边偏移
						}
					}

					if ( !((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40-1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40-1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 >= 10)
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	向右边偏移
						}
					}
				}
			}
		}
		test = done;

		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	设置移动标志
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter此处表示需要多少桢
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
				//	设置推箱子标志
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
				//	设置推箱子标志
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
		if (m_Player[m_id].x % 40 == 0)	//	判断阻挡
		{
			done = CAN_BLOCK;
			//	正前方为空
			if ((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _BZ_ ))
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					done = CAN_MOVE;			//	直接往前走
				}
				else	//	右上方有东西
				{
					if (m_Player[m_id].y%40 == 0)
					{
						done = CAN_MOVE;		//	直接往前走
					}
					else if (m_Player[m_id].y%40 <= 30)
					{
						m_Player[m_id].way = WAY_UP;
						done = CAN_MOVE;		//	向左边偏移
					}
					else if (m_Player[m_id].y%40 > 30)
					{
						if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
			}
			else		//	正前方有东西
			{
				//	右上方为空	...OK!
				if ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _KG_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _DJ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _BZ_ ))
				{

					if (m_Player[m_id].y%40 == 0)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
					else if (m_Player[m_id].y%40 >= 10)
					{
						m_Player[m_id].way = WAY_DOWN;
						done = CAN_MOVE;		//	向右边偏移
					}
					else if (m_Player[m_id].y%40 < 10)
					{
						if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
						{
							//	右上方是箱子，推
							done = CAN_PUSH;
						}
						else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
						{
							//	右上方是泡泡，踢
							done = CAN_KICK;
						}
					}
				}
				else	//	右上方有东西
				{
					//	此处较麻烦分4种情况:	1.左边可以移动，右边可以移动	如果x%40<20则左移，否则右移
					//							2.左边可以移动，右边不可移动	如果x%40<=30则左移
					//							3.左边不可移动，右边可以移动	如果x%40>=10则右移
					//							4.左边不可移动，右边不可移动	不能移动
					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 < 20)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	向左边偏移
						}
						else
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	向右边偏移
						}
					}

					if (((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& !((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 == 0)
						{
							if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ )
							{
								//	右上方是箱子，推
								done = CAN_PUSH;
							}
							else	if (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ )
							{
								//	右上方是泡泡，踢
								done = CAN_KICK;
							}
						}
						else	if (m_Player[m_id].y%40 <= 30)
						{
							m_Player[m_id].way = WAY_UP;
							done = CAN_MOVE;		//	向左边偏移
						}
					}

					if ( !((m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40][m_Player[m_id].x/40+1].type == _PP_ ))
						&& ((m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _XZ_ ) || (m_ScreenItem[m_Player[m_id].y/40+1][m_Player[m_id].x/40+1].type == _PP_ )))
					{
						if (m_Player[m_id].y%40 >= 10)
						{
							m_Player[m_id].way = WAY_DOWN;
							done = CAN_MOVE;		//	向右边偏移
						}
					}
				}
			}
		}
		test = done;

		if (done == CAN_MOVE)
		{
			m_Player[m_id].status = STATUS_MOVE;						//	设置移动标志
			m_Player[m_id].counter = m_Player[m_id].step / m_Player[m_id].speed;	//	counter此处表示需要多少桢
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
				//	设置推箱子标志
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
				//	设置推箱子标志
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
					if (m_Player[PlayerIndex].x%40 == 0 && m_Player[PlayerIndex].y%40 == 0)		//	到整格子了
					{
						m_Player[PlayerIndex].frame = 4;
						m_Player[PlayerIndex].status = STATUS_FREE;
					}
					else																		//	还没有到整格子
					{
						m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter此处表示需要多少桢
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
			case 0:				//	炸弹威力
				if (m_Player[PlayerIndex].power<7)
				{
					m_Player[PlayerIndex].power++;
				}
				break;
			case 1:				//	炸弹个数
				m_Player[PlayerIndex].bomb++;
				break;
			case 2:				//	移动速度
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
			case 3:				//	扔泡泡
				m_Player[PlayerIndex].cast = true;
				break;
			case 4:				//	踢泡泡
				m_Player[PlayerIndex].kick = true;
				break;
			case 5:				//	救命针
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

	if (flag)							//	需要找一个安全的地方
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
			if (m_ScreenItem[i][j].type == _DJ_)		//	8表示道具
			{
				int	s = i*15 + j;

				//////////////////////////////////////////////////////////////////////////

				SetGraph(PlayerIndex);
				Dijkstra(m_Graph,m_Distance,m_Path,s);

				s = (m_Player[PlayerIndex].y+20)/40*15+(m_Player[PlayerIndex].x+20)/40;

				if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	有最短路径并且不在同一个位置
				{
					int pre = m_Path[s];
					if (IsDangerous(pre%15,pre/15))
					{
						return	false;
					}
					m_Player[PlayerIndex].status = STATUS_MOVE;						//	设置移动标志
					m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter此处表示需要多少桢
					if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
					{
						m_Player[PlayerIndex].counter++;
					}

					if (pre == s-1)				//在左边
					{
						m_Player[PlayerIndex].way = WAY_LEFT;
					}
					else	if (pre == s+1)		//在右边
					{
						m_Player[PlayerIndex].way = WAY_RIGHT;
					}
					else	if (pre == s-15)	//在上边
					{
						m_Player[PlayerIndex].way = WAY_UP;
					}
					else						//在下边
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

	if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	有最短路径并且不在同一个位置
	{
		int pre = m_Path[s];
		if (IsDangerous(pre%15,pre/15))
		{
			return	false;
		}
		m_Player[PlayerIndex].status = STATUS_MOVE;						//	设置移动标志
		m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter此处表示需要多少桢
		if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
		{
			m_Player[PlayerIndex].counter++;
		}

		if (pre == s-1)				//在左边
		{
			m_Player[PlayerIndex].way = WAY_LEFT;
		}
		else	if (pre == s+1)		//在右边
		{
			m_Player[PlayerIndex].way = WAY_RIGHT;
		}
		else	if (pre == s-15)	//在上边
		{
			m_Player[PlayerIndex].way = WAY_UP;
		}
		else						//在下边
		{
			m_Player[PlayerIndex].way = WAY_DOWN;
		}

		flag = true;
	}
	//////////////////////////////////////////////////////////////////////////
	else	if(m_Distance[s] == 0)
	{
		m_PlayerLay[PlayerIndex] = true;
		m_LayX[PlayerIndex] = (m_Player[0].x+20)/40;			//	炸弹摆放位置x
		m_LayY[PlayerIndex] = (m_Player[0].y+20)/40;			//	炸弹摆放位置y
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
			if (m_ScreenItem[i][j].type == _KG_ || m_ScreenItem[i][j].type == _DJ_)		//	该顶点为空
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

		if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0 && m_Distance[s] < 10)		//	有最短路径并且不在同一个位置
		{
			int pre = m_Path[s];
			if (IsDangerous(pre%_LJ_W_,pre/_LJ_W_))
			{
				return	false;
			}
			m_Player[PlayerIndex].status = STATUS_MOVE;						//	设置移动标志
			m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter此处表示需要多少桢
			if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
			{
				m_Player[PlayerIndex].counter++;
			}

			if (pre == s-1)				//在左边
			{
				m_Player[PlayerIndex].way = WAY_LEFT;
			}
			else	if (pre == s+1)		//在右边
			{
				m_Player[PlayerIndex].way = WAY_RIGHT;
			}
			else	if (pre == s-_LJ_W_)	//在上边
			{
				m_Player[PlayerIndex].way = WAY_UP;
			}
			else						//在下边
			{
				m_Player[PlayerIndex].way = WAY_DOWN;
			}

			flag = true;
		}
		////////////////////////////////////////////////////////////////////////
		else	if(m_Distance[k] == 0)
		{
			m_PlayerLay[PlayerIndex] = true;
			m_LayX[PlayerIndex] = k%_LJ_W_;			//	炸弹摆放位置x
			m_LayY[PlayerIndex] = k/_LJ_W_;			//	炸弹摆放位置y
		}
	}

	return	flag;
}

bool CSceneGAME::IsDangerous(int x,int y)
{
	bool	flag = false;

	int	i;
	for (i=0;i<_LJ_H_;i++)					//	先判断y方向上是否有危险
	{
		if (m_ScreenItem[i][x].type == _PP_ && abs(y-i)<=m_ScreenItem[i][x].status && m_ScreenItem[i][x].counter<150)
		{
			flag = true;			//	有危险
			break;
		}
	}

	if (!flag)
	{
		for (i=0;i<_LJ_W_;i++)					//	再判断x方向上是否有危险
		{
			if (m_ScreenItem[y][i].type == _PP_ && abs(x-i)<=m_ScreenItem[y][i].status && m_ScreenItem[y][i].counter<150)
			{
				flag = true;			//	有危险
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

	if (m_Distance[s] != INFINITYUSER && m_Distance[s] != 0)		//	有最短路径并且不在同一个位置
	{
		int pre = m_Path[s];
		//		if (IsDangerous(pre%15,pre/15))
		//		{
		//			return	false;
		//		}

		m_Player[PlayerIndex].status = STATUS_MOVE;						//	设置移动标志
		m_Player[PlayerIndex].counter = m_Player[PlayerIndex].step / m_Player[PlayerIndex].speed;	//	counter此处表示需要多少桢
		if (m_Player[PlayerIndex].step % m_Player[PlayerIndex].speed != 0)
		{
			m_Player[PlayerIndex].counter++;
		}

		if (pre == s-1)				//在左边
		{
			m_Player[PlayerIndex].way = WAY_LEFT;
		}
		else	if (pre == s+1)		//在右边
		{
			m_Player[PlayerIndex].way = WAY_RIGHT;
		}
		else	if (pre == s-_LJ_W_)	//在上边
		{
			m_Player[PlayerIndex].way = WAY_UP;
		}
		else						//在下边
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
			if (i/15 == j/_LJ_W_)			//	两个顶点在同一行上
			{
				if (j%_LJ_W_ == i%_LJ_W_-1 || j%_LJ_W_ == i%_LJ_W_+1)	//	在相邻的左边或是右边
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
			else	if (i%_LJ_W_ == j%_LJ_W_)	//	两个顶点在同一列上
			{
				if (j/_LJ_W_ == i/_LJ_W_-1 || j/_LJ_W_ == i/_LJ_W_+1)	//	在相邻的上边或是下边
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
	//	G是无向网的邻接矩阵，若边(i,j)不存在，G[i][j] = INFINITYUSER
	//	D是最短距离
	//	P是最短路径
	//	设源点 0 <= s <= MaxVertexNum-1

	bool	S[MaxVertexNum];				//	红点集合，S[i]为真表示红点，否则为蓝点
	int		i,j,k,min;
	for (i=0;i<MaxVertexNum;i++)
	{
		S[i] = false;						//	置红点集为空
		D[i] = G[s][i];						//	置初始的估计距离
		if (D[i]<INFINITYUSER)					//	INFINITYUSER表示无穷大
		{
			P[i] = s;						//	s是i的前趋（双亲）
		}
		else
		{
			P[i] = -1;						//	i无前趋，P[s]亦无前趋，因为G[s][s]亦为INFINITYUSER
		}
	}

	S[s] = true;							//	红点集初始时只有源点
	D[s] = 0;								//	源点到源点的距离为0

	for (i=0;i<MaxVertexNum-1;i++)			//	扩充红点集
	{
		min = INFINITYUSER;
		for (j=0;j<MaxVertexNum;j++)		//	在当前蓝点集中选估计距离最小的顶点k，即离s最近的顶点
		{
			if (!S[j] && D[j]<min)			//	j是蓝点且D[j]小于当前最小的估计距离
			{
				min = D[j];
				k = j;
			}
		}
		if (min == INFINITYUSER)					//	蓝点集中已无估计距离小于INFINITYUSER的顶点
		{
			return;
		}
		S[k] = true;						//	k加入红点集

		for (j=0;j<MaxVertexNum;j++)		//	调整剩余蓝点的估计距离
		{
			if (!S[j] && (D[j] > D[k] + G[k][j]))
			{
				D[j] = D[k] + G[k][j];		//	修改蓝点j的估计距离，使j离s更近
				P[j] = k;					//	k是j的前趋
			}
		}
	}
}

int CSceneGAME::GetDestroyNumber(int PlayerIndex,int x,int y)
{
	int	counter = 0;
	int	i;
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	检查上边
	{
		if ((y-i)>=0 && (m_ScreenItem[y-i][x].type == _MK_ || m_ScreenItem[y-i][x].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	检查下边
	{
		if ((y+i)<_LJ_H_ && (m_ScreenItem[y+i][x].type == _MK_ || m_ScreenItem[y+i][x].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	检查左边
	{
		if ((x-i)>=0 && (m_ScreenItem[y][x-i].type == _MK_ || m_ScreenItem[y][x-i].type == _XZ_))
		{
			counter++;
			break;
		}
	}
	for (i=0;i<=m_Player[PlayerIndex].power;i++)		//	检查右边
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
	//	上边
	for (k=1;k<m_ScreenItem[i][j].status;k++)
	{
		if ( (i-k)<0 ||(m_ScreenItem[i-k][j].type == _MK_) || (m_ScreenItem[i-k][j].type == _XZ_) || (m_ScreenItem[i-k][j].type == _MZ_) || (m_ScreenItem[i-k][j].type == _QZ_))
		{
			break;							//	如果是木箱或是木块则退出该循环
		}
		if (m_ScreenItem[i-k][j].type == _PP_)
		{
			m_ScreenItem[i-k][j].counter = 1; 
			break;							//	如果是泡泡则将计数器置为1下个循环就会爆炸
		}
		if (m_ScreenItem[i-k][j].type == _DJ_)
		{
			m_ScreenItem[i-k][j].tag = -1;
		}
		m_ScreenItem[i-k][j].type = _BZ_;
		m_ScreenItem[i-k][j].counter = 60; 
		m_ScreenItem[i-k][j].status = 1;
	}
	//	最上边
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
	//	下边
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
	//	最下边
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
	//	左边
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
	//	最左边
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
	//	右边
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
	//	最右边
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
	//	中心处
	m_ScreenItem[i][j].type = _BZ_;
	m_ScreenItem[i][j].counter = 60;
	m_ScreenItem[i][j].status = 0;

	m_Player[m_ScreenItem[i][j].who].bomb++;						//	炸弹数恢复

}

void CSceneGAME::DoKick()
{
	RECT	rect;
	for (list<MOVEBOMB>::iterator iter = m_bombKick.begin(); iter != m_bombKick.end(); iter++)
	{

		//	判断是否到达目标点，如果到达，从链表中删除该元素，将泡泡加入到背景信息中
		bool	deleteflag = false;
		//	踢出来的泡泡(前方有阻挡就停止运动)
		switch(iter->way)
		{
		case WAY_UP:
			if (iter->y%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->y == 0 || m_ScreenItem[iter->y/40-1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40-1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_DOWN:
			if (iter->y%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->y == 12*40 || m_ScreenItem[iter->y/40+1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40+1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_LEFT:
			if (iter->x%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->x == 0 || m_ScreenItem[iter->y/40][iter->x/40-1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40-1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_RIGHT:
			if (iter->x%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->x == 14*40 || m_ScreenItem[iter->y/40][iter->x/40+1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40+1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = iter->counter;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
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

		//	踢出来的泡泡(每桢移动5个像素)
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

		//	判断是否到达目标点，如果到达，从链表中删除该元素，将泡泡加入到背景信息中
		bool	deleteflag = false;
		//	扔出来的泡泡(到达目标点之后若目标点上无东西，则停止，否则重新计算目标点)
		switch(iter->way)
		{
		case WAY_UP:
			if (iter->y%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->y == 0 || m_ScreenItem[iter->y/40-1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40-1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_DOWN:
			if (iter->y%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->y == 12*40 || m_ScreenItem[iter->y/40+1][iter->x/40].type > 0 && m_ScreenItem[iter->y/40+1][iter->x/40].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_LEFT:
			if (iter->x%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->x == 0 || m_ScreenItem[iter->y/40][iter->x/40-1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40-1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
				}
			}
			break;
		case WAY_RIGHT:
			if (iter->x%40 == 0)
			{
				//	此处检测前方有无玩家，若有则停止前进

				//	此处检测前方有无阻挡物，若有则停止前进
				if (iter->x == 14*40 || m_ScreenItem[iter->y/40][iter->x/40+1].type > 0 && m_ScreenItem[iter->y/40][iter->x/40+1].type <8)
				{
					m_ScreenItem[iter->y/40][iter->x/40].type = _PP_;
					m_ScreenItem[iter->y/40][iter->x/40].counter = 180;
					m_ScreenItem[iter->y/40][iter->x/40].tag = -1;
					m_ScreenItem[iter->y/40][iter->x/40].status = iter->power;
					m_ScreenItem[iter->y/40][iter->x/40].who = iter->who;
					deleteflag = true;		//	设置删除标志
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

		//	扔出来的泡泡(每桢移动1个像素)
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
		//	控制泡泡上下飘动的参数
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

	//	清除背景
	PGF->GetGO()->tDrawBmp(0, 0, 800, 600,0,0, "bmp\\pic.bmp",RGB(255,0,255));

	//	地面
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

	//	显示物体
	for(i=0;i<_LJ_H_;i++)
	{
		for(j=_LJ_W_ - 1;j>=0;j--)
		{
			switch(m_ScreenItem[i][j].type)
			{
			case _MK_:					//	木块
				rect.left = 120;
				rect.top = 120;
				rect.right = 160;
				rect.bottom = 200;
				PGF->GetGO()->tDrawBmp(j*40+19, i*40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				if (j!=0)			//	左边阴影
				{
					rect.left = 480;
					rect.top = 348;
					rect.right = 480+5;
					rect.bottom = 348+39;
					PGF->GetGO()->tDrawBmp(j*40+19-5, i*40+40+1, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				if (i!=12)			//	下边阴影
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
			case _XZ_:					//	箱子

				//	判断箱子是否在运动，若是修改其坐标偏移量
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

				if (j!=0)			//	左边阴影
				{
					rect.left = 480;
					rect.top = 348;
					rect.right = 480+5;
					rect.bottom = 348+39;
					PGF->GetGO()->tDrawBmp(j*40+19-5+m_ScreenItem[i][j].xoffset, i*40+40+1+m_ScreenItem[i][j].yoffset, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				if (i!=12)			//	下边阴影
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
			case _MZ_:					//	木桩
				rect.left = 200;
				rect.top = 120;
				rect.right = 240;
				rect.bottom = 200;
				PGF->GetGO()->tDrawBmp(j*40+19, i*40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				break;
			case _QZ_:					//	海盗旗
				if (m_ScreenItem[i][j].status == 0)
				{
					rect.left = 240;
					rect.top = 40;
					rect.right = 440;
					rect.bottom = 240;
					PGF->GetGO()->tDrawBmp(5*40+19, 2*40+40, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				}
				break;
			case _PP_:					//	泡泡（静止的，相对于移动的而言）
				rect.left = 480+m_ScreenItem[i][j].counter/30%3*44;
				rect.top = 304;
				rect.right = 480+44+m_ScreenItem[i][j].counter/30%3*44;
				rect.bottom = 304+44;
				PGF->GetGO()->tDrawBmp(j*40+19-2, i*40+40-4, rect.right - rect.left,rect.bottom - rect.top,rect.left,rect.top, "bmp\\pic.bmp",RGB(255,0,255));

				m_ScreenItem[i][j].counter--;
				if (m_ScreenItem[i][j].counter <= 0)		//	泡泡爆炸
				{
					SetBlast(i,j);
					sound.play(DETONATION);		//	播放爆炸音效
				}
				break;
			case _DJ_:					//	道具
				if (m_ScreenItem[i][j].counter % 10 == 0)
				{
					if (m_ScreenItem[i][j].status == 0)			//	加
					{
						m_ScreenItem[i][j].yoffset++;
						if (m_ScreenItem[i][j].yoffset == 3)
						{
							m_ScreenItem[i][j].status = -1;
						}
					}
					else										//	减
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

				//	tag表示道具的种类
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
			case _BZ_:											//	爆炸效果
				{
					//	status在此处表示爆炸效果的编号
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
					if (m_ScreenItem[i][j].counter == 0)		//	爆炸结束
					{
						if (m_ScreenItem[i][j].tag != -1)
						{
							m_ScreenItem[i][j].type = _DJ_;		//	炸出来的道具
							sound.play(APPEAR);		//	播放道具出现音效
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

		//	显示玩家	DrawPlayer
		for (int PlayerIndex=0;PlayerIndex<PLAYER_NUM;PlayerIndex++)
		{
			if ((m_Player[PlayerIndex].y+20)/40 == i && m_Player[PlayerIndex].type != TYPE_NULL)
			{
				if (m_Player[PlayerIndex].status == STATUS_DIED)				//	被炸？
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
		sprintf_s(str,"FPS: %f 主角1信息 x: %d y: %d way: %d speed: %d",m_fps,m_Player[0].x,m_Player[0].y,m_Player[0].way,m_Player[0].speed);
		PGF->GetGO()->DrawText(str,19, 40, RGB(255,255,0));
		if(DS)
		{
			sprintf_s(str,"FPS: %f 主角2信息 x: %d y: %d way: %d speed: %d",m_fps,m_Player[1].x,m_Player[1].y,m_Player[1].way,m_Player[1].speed);
			PGF->GetGO()->DrawText(str,19, 60, RGB(255,255,0));

		}
		else
		{
			sprintf_s(str,"FPS: %f 电脑信息 x: %d y: %d way: %d speed: %d",m_fps,m_Player[1].x,m_Player[1].y,m_Player[1].way,m_Player[1].speed);
			PGF->GetGO()->DrawText(str,19, 60, RGB(255,255,0));

		}
		sprintf_s(str,"F1 隐藏");
		PGF->GetGO()->DrawText(str,19, 80, RGB(255,255,0));

		sprintf_s(str,"F2 重来");
		PGF->GetGO()->DrawText(str,19, 100, RGB(255,255,0));

		sprintf_s(str,"F3 返回");
		PGF->GetGO()->DrawText(str,19, 120, RGB(255,255,0));

		sprintf_s(str,"1 静音");
		PGF->GetGO()->DrawText(str,19, 140, RGB(255,255,0));

		sprintf_s(str,"2 加音");
		PGF->GetGO()->DrawText(str,19, 160, RGB(255,255,0));

		sprintf_s(str,"3 减音");
		PGF->GetGO()->DrawText(str,19, 180, RGB(255,255,0));

	}


	//	显示小图标
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
				PGF->GetGO()->DrawText("玩家一",705, 87+i*44, RGB(255,255,0));
			else
			{
				if(DS)
					PGF->GetGO()->DrawText("玩家二",705, 87+i*44, RGB(255,255,0));
				else
					PGF->GetGO()->DrawText("电脑玩家",705, 87+i*44, RGB(255,255,0));
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
		PGF->SetCurScene("开始场景");
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


	//	以下5个函数用于处理电脑玩家的人工智能(每移动一个格子判断一次，因为背景上的信息随时在改变)
	else
	{
		for (int i=0;i<PLAYER_NUM;i++)
		{
			if (m_Player[i].type == TYPE_COMPUTER && m_Player[i].status == STATUS_FREE)
			{
				//	已经在整个格子上了，m_MoveCount[i]在走之前设置，走完一步之后减1

				if (!FindSafety(i))							//	查找一个安全的位置，找到后去躲藏
				{
					//	目前所站位置是安全的
					//				if (!FindTeammateForRescue(i))			//	查找需要营救的队友的位置
					{
						if (!FindPropertyForGet(i))			//	查看道具的位置，若找到则去吃
						{
							if (!FindAdversaryForAttack(i))	//	查找对手的位置，找到后去攻击
							{
								if (!FindLay(i))				//	查找一个放置炸弹的位置,以便炸出一些道具
								{

								}
							}
						}
					}
				}

				//判断是否要放炸弹
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