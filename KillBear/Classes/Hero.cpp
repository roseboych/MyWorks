#include "Hero.h"

Hero::Hero(void)
{
	global->hero = this;
}
Hero::~Hero(void)
{
}

bool Hero::init(){
	bool ret = false;
	do 
	{
		this->initWithSpriteFrameName("boy_idle_00.png");
		//this->setAnchorPoint(Vec2(0.48f, 0.13f));

		//վ��ʱ���Ŷ���
		Animation *idleAnim = this->createNomalAnimation("boy_idle_%02d.png", 3, 5);
		this->setIdleAction(RepeatForever::create(Animate::create(idleAnim)));

		Animation *walkAnim = this->createNomalAnimation("boy_run_%02d.png", 8, 12);
		this->setWalkAction(RepeatForever::create(Animate::create(walkAnim)));

		//��ͨ����A,�ֳ��к�����,�ڼ���ӹ����ж�.�Լ�����ͨ������fps���Ƴ����ٶ�֮���
		Animation *attackAnima1 = this->createAttackAnimation("boy_attack_00_%02d.png", 0, 4, 10);
		Animation *attackAnima2 = this->createAttackAnimation("boy_attack_00_%02d.png", 4, 8, 15);
		this->setNomalAttackA(Sequence::create(
			Animate::create(attackAnima1), 
			CallFuncN::create(CC_CALLBACK_1(Hero::attackCallBackAction,this)),
			Animate::create(attackAnima2),
			Role::createIdleCallbackFunc(),
			NULL));

		Animation *attackAnimb1 = this->createAttackAnimation("boy_attack_01_%02d.png", 0, 3, 10);
		Animation *attackAnimb2 = this->createAttackAnimation("boy_attack_01_%02d.png", 3, 7, 15);
		this->setNomalAttackB(Sequence::create(
			Animate::create(attackAnimb1), 
			CallFuncN::create(CC_CALLBACK_1(Hero::attackCallBackAction,this)),
			Animate::create(attackAnimb2),
			Role::createIdleCallbackFunc(),
			NULL));

		Animation *attackAnimc1 = this->createAttackAnimation("boy_attack_02_%02d.png", 0, 3, 10);
		Animation *attackAnimc2 = this->createAttackAnimation("boy_attack_02_%02d.png", 3, 6, 12);
		this->setNomalAttackC(Sequence::create(
			Animate::create(attackAnimc1), 
			CallFuncN::create(CC_CALLBACK_1(Hero::attackCallBackAction,this)),
			Animate::create(attackAnimc2),
			Role::createIdleCallbackFunc(),
			NULL));

		Animation *chageAnim = this->createNomalAnimation("boy_change_attack_%02d.png", 4, 5);
		this->setChange(RepeatForever::create(Animate::create(chageAnim)));

		Animation *chageattAnim1= this->createAttackAnimation("boy_change_attack_%02d.png", 5, 7 , 2);
		Animation *chageattAnim2= this->createAttackAnimation("boy_change_attack_%02d.png", 7, 12 , 5);
		this->setChangeAttack(Sequence::create(
			Animate::create(chageattAnim1), 
			CallFuncN::create(CC_CALLBACK_1(Hero::attackCallBackAction,this)),
			Animate::create(chageattAnim2),
			Role::createIdleCallbackFunc(),
			NULL));

		Animation *gurtAnim = this->createNomalAnimation("boy_hurt_%02d.png", 2, 4);
		this->setHurtAction(Sequence::create(
			Animate::create(gurtAnim), 
			Role::createIdleCallbackFunc(), 
			NULL));

		Animation *deadAnim = this->createNomalAnimation("boy_dead_%02d.png", 3, 6);
		this->setDeadAction(Sequence::create(
			Animate::create(deadAnim), 
			Blink::create(3, 9), 
			NULL));

		//��ײ�����趨
		this->m_bodyBox = this->createBoundingBox(Vec2(0,30), Size(30,60));
		this->m_hitBox = this->createBoundingBox(Vec2(35,50), Size(80, 90));


		ret = true;
	} while(0);

	return ret;
}

void Hero::onMove(Vec2 direction, float distance)//�ƶ�����
{
	this->setFlippedX(direction.x < 0 ? true : false);
	this->runWalkAction();
	Vec2 velocity = direction * (distance < 33 ? 1 : 3);
	this->setVelocity(velocity);
}
void Hero::onStop()//վ��
{
	this->runIdleAction();
	this->setVelocity(Vec2::ZERO);
}
void Hero::onAttack(int number)//ִ�й���
{
	this->runNomalAttackA();
}
void Hero::updateSelf()//ˢ���Լ�
{
	if(this->getCurrActionState() == ACTION_STATE_WALK)
	{
		Vec2 currentP= this->getPosition();				//��ǰ����
		Vec2 expectP = currentP + this->getVelocity();	//��������
		Vec2 actualP = expectP;							//ʵ������
		float mapWidth = global->tileMap->getContentSize().width;	//���ŵ�ͼ���
		float herofat = this->getBodyBox().actual.size.width/2;		//��ɫ������,���ܹ�����bodyboxΪ׼
		////�����ܵ�ǽ��ȥ
		//if(expectP.y<0 || !global->tileAllowMove(expectP))
		float maptileHeight = global->tileMap->getTileSize().height;
		if(expectP.y < 0 || expectP.y > maptileHeight * 3 )
		{
			actualP.y =currentP.y;
		}
		//�����ܳ���ͼ����
		if(expectP.x < herofat || expectP.x >= mapWidth - herofat)
		{
			//if(!global->tileAllowMove(expectP))
			actualP.x = currentP.x;
		}
		this->setPosition(actualP);
		this->setLocalZOrder( Director::getInstance()->getVisibleSize().height - this->getPositionY());
	}
}
void Hero::attackCallBackAction(Node* pSender)
{//��ͨ�����ص�
	__Array* pEnemies = global->enemies ;
	Ref *enemyObj = NULL;
	//�������й��� 
	CCARRAY_FOREACH(pEnemies, enemyObj)
	{
		Enemy *pEnemy = (Enemy*)enemyObj;
		if(fabsf(this->getPosition().y - pEnemy->getPosition().y) < 20)
		{
			Rect attackReck = m_hitBox.actual;//Ӣ�۹�������
			Rect hurtReck = pEnemy->getBodyBox().actual;;//������������
			if(attackReck.intersectsRect(hurtReck))
			{
				pEnemy->setAllowMove(false);
				//�˴��ɴ�����ﱻ������ĺ���,�����˺�����
				int damage = random(this->getDamageStrenth()*0.7,this->getDamageStrenth()*1.3);
				damageDisplay(damage , pEnemy->getBodyBox().actual.origin);
				pEnemy->runHurtAction();
				pEnemy->setCurtLifeValue(pEnemy->getCurtLifeValue() - damage);
				if(pEnemy->getCurtLifeValue() <= 0)
				{
					pEnemy->runDeadAction();
					pEnemy->setBodyBox(createBoundingBox(Vec2::ZERO, Size::ZERO));
				}
			}
		}
	}
	//this->runIdleAction();
}
void Hero::FontsCallBackAction(Node* pSender)
{
	//���ֵ����ص�
	global->gameLayer->removeChild(pSender);
}
void Hero::damageDisplay(int number,Vec2 point)
{
	//�������ֶ���
	auto atLabel = Label::create();
	//char ch[100]={0};
	//sprintf(ch,"-%d",number);
	atLabel->setString(__String::createWithFormat("-%d",number)->getCString());
	atLabel->setSystemFontSize(18);
	atLabel->setColor(Color3B(0,0,128));
	atLabel->setPosition(point);
	global->gameLayer->addChild(atLabel,200);
	FiniteTimeAction * callFuncN = CallFuncN::create(atLabel, callfuncN_selector(Hero::FontsCallBackAction));
	FiniteTimeAction *sequence = Sequence::create(
		//FadeIn::create(1.5f),
		ScaleTo::create(0.2f,1.3f),
		MoveBy::create(0.2f,Vec2(0,20)),
		FadeOut::create(0.5f),
		callFuncN,
		NULL); 
	atLabel->runAction(sequence);
}