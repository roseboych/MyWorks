#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
USING_NS_CC;
#include "Global.h"
#include "Role.h"

//������
class Hero :public Role
{
public:
	Hero(void);
	~Hero(void);
	virtual bool init();
	CREATE_FUNC(Hero);
	void onMove(Vec2 direction, float distance);
	void onStop();
	void onAttack(int number);
	void updateSelf();
	void Hero::attackCallBackAction(Node* pSender);//��ͨ�����ص�
	void Hero::FontsCallBackAction(Node* pSender);//���ֵ����ص�
	void Hero::damageDisplay(int number,Vec2 point);//�����˺����ֶ���
};

#endif

