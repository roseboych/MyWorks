#ifndef _ROLE_H_
#define _ROLE_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct _BoundingBox
{
	Rect actual;
	Rect original;
}BoundingBox;

typedef enum {
	ACTION_STATE_NONE = 0,
	ACTION_STATE_IDLE,
	ACTION_STATE_WALK,
	ACTION_STATE_RUN,
	ACTION_STATE_JUMP,
	ACTION_STATE_NOMAL_ATTACK_A,
	ACTION_STATE_NOMAL_ATTACK_B,
	ACTION_STATE_NOMAL_ATTACK_C,
	ACTION_STATE_NOMAL_ATTACK_D,
	ACTION_STATE_CHANGE,
	ACTION_STATE_CHANGE_ATTACK,
	ACTION_STATE_RUSH_ATTACK,
	ACTION_STATE_JUMP_ATTACK_A,
	ACTION_STATE_JUMP_ATTACK_B,
	ACTION_STATE_SKILL_ATTACK_A,
	ACTION_STATE_SKILL_ATTACK_B,
	ACTION_STATE_SKILL_ATTACK_C,
	ACTION_STATE_HURT,
	ACTION_STATE_DEAD
}ActionState;
//������ɫ�࣬���Ǻ�NPC����Ҫ�̳���
//�����Ŀ��Ʋ��Ŷ���,��ʱ�������ھ��������������
class Role :public Sprite
{
public:
	Role(void);
	~Role(void);
	/* ��ɫ״̬�趨,��ʼ����ɫ״̬�� */
	CC_SYNTHESIZE(std::string,Name,Name);							//��ɫ����
	CC_SYNTHESIZE(float,curtLifevalue,CurtLifeValue);				//��ɫ��ǰ����ֵ
	CC_SYNTHESIZE(float,sumLifevalue,SumLifeValue);					//��ɫ��������ֵ
	CC_SYNTHESIZE(float,damagestrenth,DamageStrenth);				//��ɫ��ǰ������
	CC_SYNTHESIZE(Vec2, velocity, Velocity);						//��ɫ�ƶ��ٶ�
	//CC_SYNTHESIZE(Direction, roleDirection, RoleDirection);		//��ɫ����(������������)
	//CC_SYNTHESIZE(CCPoint, _vector, Vector);						//ƫ������AI�Զ��ƶ�ʱ��һ֡��ƫ������
	CC_SYNTHESIZE(bool, allowmove, AllowMove);						//��ɫ�Ƿ������ƶ������磺���������˵ȶ���ִ���ڼ䲻���ƶ�
	CC_SYNTHESIZE(ActionState,currActionState,CurrActionState);		//��ǰAction״̬(�ݴ�״̬�����������֮����ν�����)

	/* ��Ҫ��creatNomalAnimation����,ֻ����ͨ�������������ж����޹����ж� */
	CC_SYNTHESIZE_RETAIN(Action*, m_pidleaction, IdleAction);		//��ɫ����ʱ����
	CC_SYNTHESIZE_RETAIN(Action*, m_pwalkaction, WalkAction);		//��ɫ�ƶ�ʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_prunaction,  RunAction );		//��ɫ�ܶ�ʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pjumpaction, JumpAction);		//��ɫ��Ծʱ����֡����

	/* �����ǹ������ж�������Ҫ��creatAttackAnimation���������ڶ���ִ���м���ӹ����ж�������Ʈ�ֵ�*/
	CC_SYNTHESIZE_RETAIN(Action*, m_pnomalattacka, NomalAttackA);	//��ɫ��ͨ����Aʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pnomalattackb, NomalAttackB);	//��ɫ��ͨ����Bʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pnomalattackc, NomalAttackC);	//��ɫ��ͨ����Cʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pnomalattackd, NomalAttackD);	//��ɫ��ͨ����Dʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pchange,  Change);				//��ɫ����ʱ�Ķ�������
	CC_SYNTHESIZE_RETAIN(Action*, m_pchangeattack,  ChangeAttack);	//��ɫ��������ʱ�Ķ�������

	CC_SYNTHESIZE_RETAIN(Action*, m_prushattack,   RushAttack  );	//��ɫ�ܶ�����ʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pjumpattacka,  JumpAttackA );	//��ɫ��Ծ����Aʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pjumpattackb,  JumpAttackb );	//��ɫ��Ծ����Bʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pskillattacka, SkillAttackA);	//��ɫ���ܹ���Aʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pskillattackb, SkillAttackB);	//��ɫ���ܹ���Bʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pskillattackc, SkillAttackC);	//��ɫ���ܹ���Cʱ����֡����

	/*������createImmortalAnimation����,ֻ�ж���Ч��,�������ж�,Ҳ��������ʱ�򲥷Ŷ������޵е�,���һ������ж���,���˾�ȥ����*/
	CC_SYNTHESIZE_RETAIN(Action*, m_phurtaction, HurtAction);		//��ɫ����ʱ����֡����
	CC_SYNTHESIZE_RETAIN(Action*, m_pdeadaction, DeadAction);		//��ɫ����ʱ����֡����

	void Role::callBackAction(Node* pSender);						//����ִ����ϵ�ͨ�ûص�����

	/*	���������еĶ����ص�����������ִ�д˶���,	*/
	virtual void runIdleAction();								//ִ�����ö���
	virtual void runWalkAction();								//ִ���ƶ����߶���
	virtual void runRunAction();								//ִ���ܶ����߶���
	virtual void runJumpAction();								//ִ����Ծ��������
	virtual void runNomalAttackA();								//ִ����ͨ����A����
	virtual void runNomalAttackB();								//ִ����ͨ����B����
	virtual void runNomalAttackC();								//ִ����ͨ����C����
	virtual void runNomalAttackD();								//ִ����ͨ����D����
	virtual void runChange();									//ִ����������
	virtual void runChangeAttack();								//ִ��������������
	virtual void runRushAttack();								//ִ���ܶ���������
	virtual void runJumpAttackA();								//ִ����Ծ����A����
	virtual void runJumpAttackB();								//ִ����Ծ����B����
	virtual void runSkillAttackA(); 							//ִ�м��ܹ���A����
	virtual void runSkillAttackB(); 							//ִ�м��ܹ���B����
	virtual void runSkillAttackC(); 							//ִ�м��ܹ���C����
	virtual void runHurtAction();								//ִ�б����������˶���
	virtual void runDeadAction();								//ִ������������
	CallFunc* createIdleCallbackFunc();							//�ص����ö���

	BoundingBox createBoundingBox(Vec2 origin,Size size);
	CC_SYNTHESIZE(BoundingBox, m_bodyBox, BodyBox);				//������ײ����
	CC_SYNTHESIZE(BoundingBox, m_hitBox, HitBox);				//������ײ����
	virtual void setPosition(const Vec2 &position);

	void updateBoxes();
protected:
	static Animation* createNomalAnimation(const char* formatStr, int frameCount, int fps);
	static Animation* createAttackAnimation(const char* formatStr, int frameCountBegan, int frameCountEnd,int fps);
private:
	bool changeState(ActionState actionState);
	ActionState proActionState;
};
#endif