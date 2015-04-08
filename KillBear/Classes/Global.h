#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "cocos2d.h"
USING_NS_CC; 

#include "Singleton.h"
#include "GameLayer.h"
#include "OperateLayer.h"
#include "StateLayer.h"

//�����������࣬��������Щ���з��ʵ�������ᱨ��
class GameLayer;
class OperateLayer;
class StateLayer;
class Hero;
class Enemy;

//ȫ�ֵ���
class Global :public Singleton<Global>
{
public:
	Global(void);
	~Global(void);

	//GameScene *gameScene;

	GameLayer *gameLayer;			//��Ϸ��
	OperateLayer *operateLayer;		//������
	StateLayer * stateLayer;		//״̬��
	Hero *hero;						//Ӣ��
	__Array *enemies;				//����
	TMXTiledMap *tileMap;			//��ͼ

	Point tilePosFromLocation(Vec2 MovePoint, TMXTiledMap *map = NULL);//��pointת���ɵ�ͼGID��point
	bool  tileAllowMove(Vec2 MovePoint);
};

#define global Global::instance()

#endif