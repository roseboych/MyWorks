#ifndef _GONGXIANG_H_
#define _GONGXIANG_H_

#define _KG_   0   //空地
#define _MK_   1   //木块
#define _XZ_   2   //箱子
#define _MZ_   3   //木桩
#define _QZ_   4   //海盗旗
#define _PP_   7   //泡泡
#define _DJ_   8   //道具
#define _BZ_   9   //爆炸
 
#define _LJ_W_ 15  //逻辑宽
#define _LJ_H_ 13  //逻辑高

#define _PLAY_UP_     1 //向上移动
#define _PLAY_DOWN_   2 //向下移动
#define _PLAY_LEFT_   3 //向左移动
#define _PLAY_RIGHT_  4 //向右移动
#define _PLAY_CTRL_   5 //放泡泡

#define _YL_   0.5f //音量初始化

#define  _XZ_JL_ 30 //是箱子几率
#define  _DJ_JL_ 30 //是道具几率
#define  _MK_JL_ 80 //是木块几率
#define  _DJ_ZL_ 5  //道具种类
//	运动的方向
#define		WAY_UP			1	
#define		WAY_DOWN		0
#define		WAY_LEFT		3
#define		WAY_RIGHT		2

//	玩家的状态
#define		STATUS_FREE		0			
#define		STATUS_MOVE		1
#define		STATUS_DIED		2
#define		STATUS_DELAY	3

//	玩家可执行的动作
#define		CAN_BLOCK		0			
#define		CAN_MOVE		1
#define		CAN_PUSH		2
#define		CAN_KICK		3

//	玩家的身份标识
#define		TYPE_NULL		0			
#define		TYPE_USER		1
#define		TYPE_USEROTHER	2
#define		TYPE_COMPUTER	3

#define		MaxVertexNum	13*15
#define		PLAYER_NUM		2
#define		INFINITYUSER		1000		//	无限大  大于13*15即可


//	音效的编号
#define		START			0
#define		WIN				1
#define		DEUCE			2
#define		LOST			3
#define		LAY				4
#define		DETONATION		5
#define		APPEAR			6
#define		GET				7
#define		DIE				8
#define    _BJ              9
#define    _KS              10
#define    _BZ              11

#define		GAME_FREE		0
#define		GAME_START		1
#define		GAME_WIN		2
#define		GAME_DEUCE		3
#define		GAME_LOST		4

#define		DISPLAYCOUNTER	300

struct PLAYER
{
	int		x;			//	x坐标(以像素为单位)
	int		y;			//	y坐标(以像素为单位)
	int		way;		//	主角方向
	int		bomb;		//	炸弹数
	bool	kick;		//	能踢泡泡
	bool	cast;		//	投掷、扔泡泡
	bool	pin;		//	救命针
	int		speed;		//	速度(像素/帧，取值范围 1 -- 10)
	int		step;		//	步长
	int		frame;		//	动画贞数
	int		counter;	//	计数器
	int		status;		//	状态
	int		power;		//	泡泡的威力
	int		type;		//	主角类型
	int		presswaykey;//	连续按住方向键计数(因为推箱子或是踢泡泡需要一段时间)
	int		image;		//	图像编号
};

struct ITEM
{
	int	type;			//	物体的类型
	int	status;			//	当前的状态，针对不同的物体，有不同的解释
	int	xoffset;		//	针对箱子  表示x偏移，针对爆炸效果表示爆炸效果当前的长度
	int	yoffset;		//	针对箱子  表示y偏移
	int	way;			//	移动时的方向（针对箱子）
	int	counter;		//	计数器（针对箱子）
	int	move;			//	是否移动（针对箱子）
	int	tag;			//	附属品
	int	who;			//	此泡泡是谁摆放的
};

struct	MOVEBOMB
{
	int	x;				//	x坐标(以像素为单位)
	int	y;				//	y坐标(以像素为单位)
	int	way;			//	运动方向
	int	mode;			//	0:踢,1:扔
	int	tox;			//	目标点x坐标,如果是踢的话则此参数无意义，扔的话则事先算好目标点当落下时如果发现有东西，则再算一次
	int	toy;			//	目标点y坐标
	int	counter;		//	计数器
	int	offset;			//	扔泡泡的时候y方向上的偏移
	int	power;			//	威力
	bool addorsub;	    //	控制偏移量增加或是减少,true表示增加
	int	who;			//	此泡泡是谁摆放的
};

#endif