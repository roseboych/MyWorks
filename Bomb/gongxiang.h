#ifndef _GONGXIANG_H_
#define _GONGXIANG_H_

#define _KG_   0   //�յ�
#define _MK_   1   //ľ��
#define _XZ_   2   //����
#define _MZ_   3   //ľ׮
#define _QZ_   4   //������
#define _PP_   7   //����
#define _DJ_   8   //����
#define _BZ_   9   //��ը
 
#define _LJ_W_ 15  //�߼���
#define _LJ_H_ 13  //�߼���

#define _PLAY_UP_     1 //�����ƶ�
#define _PLAY_DOWN_   2 //�����ƶ�
#define _PLAY_LEFT_   3 //�����ƶ�
#define _PLAY_RIGHT_  4 //�����ƶ�
#define _PLAY_CTRL_   5 //������

#define _YL_   0.5f //������ʼ��

#define  _XZ_JL_ 30 //�����Ӽ���
#define  _DJ_JL_ 30 //�ǵ��߼���
#define  _MK_JL_ 80 //��ľ�鼸��
#define  _DJ_ZL_ 5  //��������
//	�˶��ķ���
#define		WAY_UP			1	
#define		WAY_DOWN		0
#define		WAY_LEFT		3
#define		WAY_RIGHT		2

//	��ҵ�״̬
#define		STATUS_FREE		0			
#define		STATUS_MOVE		1
#define		STATUS_DIED		2
#define		STATUS_DELAY	3

//	��ҿ�ִ�еĶ���
#define		CAN_BLOCK		0			
#define		CAN_MOVE		1
#define		CAN_PUSH		2
#define		CAN_KICK		3

//	��ҵ���ݱ�ʶ
#define		TYPE_NULL		0			
#define		TYPE_USER		1
#define		TYPE_USEROTHER	2
#define		TYPE_COMPUTER	3

#define		MaxVertexNum	13*15
#define		PLAYER_NUM		2
#define		INFINITYUSER		1000		//	���޴�  ����13*15����


//	��Ч�ı��
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
	int		x;			//	x����(������Ϊ��λ)
	int		y;			//	y����(������Ϊ��λ)
	int		way;		//	���Ƿ���
	int		bomb;		//	ը����
	bool	kick;		//	��������
	bool	cast;		//	Ͷ����������
	bool	pin;		//	������
	int		speed;		//	�ٶ�(����/֡��ȡֵ��Χ 1 -- 10)
	int		step;		//	����
	int		frame;		//	��������
	int		counter;	//	������
	int		status;		//	״̬
	int		power;		//	���ݵ�����
	int		type;		//	��������
	int		presswaykey;//	������ס���������(��Ϊ�����ӻ�����������Ҫһ��ʱ��)
	int		image;		//	ͼ����
};

struct ITEM
{
	int	type;			//	���������
	int	status;			//	��ǰ��״̬����Բ�ͬ�����壬�в�ͬ�Ľ���
	int	xoffset;		//	�������  ��ʾxƫ�ƣ���Ա�ըЧ����ʾ��ըЧ����ǰ�ĳ���
	int	yoffset;		//	�������  ��ʾyƫ��
	int	way;			//	�ƶ�ʱ�ķ���������ӣ�
	int	counter;		//	��������������ӣ�
	int	move;			//	�Ƿ��ƶ���������ӣ�
	int	tag;			//	����Ʒ
	int	who;			//	��������˭�ڷŵ�
};

struct	MOVEBOMB
{
	int	x;				//	x����(������Ϊ��λ)
	int	y;				//	y����(������Ϊ��λ)
	int	way;			//	�˶�����
	int	mode;			//	0:��,1:��
	int	tox;			//	Ŀ���x����,������ߵĻ���˲��������壬�ӵĻ����������Ŀ��㵱����ʱ��������ж�����������һ��
	int	toy;			//	Ŀ���y����
	int	counter;		//	������
	int	offset;			//	�����ݵ�ʱ��y�����ϵ�ƫ��
	int	power;			//	����
	bool addorsub;	    //	����ƫ�������ӻ��Ǽ���,true��ʾ����
	int	who;			//	��������˭�ڷŵ�
};

#endif