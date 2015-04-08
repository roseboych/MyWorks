#include "Joystick.h"
using namespace std;

Joystick::Joystick()
{
}

Joystick::~Joystick()
{
}

bool Joystick::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Sprite::init() );
		m_pJoystickBg = Sprite::create("JoystickBg.png");//����
		m_pJoystick = Sprite::create("Joystick.png");//ҡ��
		this->addChild(m_pJoystickBg, 0);
		this->addChild(m_pJoystick, 1);
		this->hideJoystick();
		//this->showJoystick();
		m_pJoystickR= m_pJoystickBg->getContentSize().width/2;
		m_pJoystickr= m_pJoystick->getContentSize().width/2;

		//�µ�APIע����ôд
		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(Joystick::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(Joystick::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(Joystick::onTouchesEnded, this); 
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		ret = true;
	} while(0);

	return ret;
}

void Joystick::showJoystick()
{
	//��ʾҡ��
	m_pJoystick->setVisible(true); 
	m_pJoystickBg->setVisible(true);
}

void Joystick::hideJoystick()
{
	//����ҡ��
	//m_pJoystick->setPosition(m_pJoystickBg->getPosition());
	m_pJoystick->setVisible(false);
	//m_pJoystickBg->setVisible(false);
	m_pJoystickBg->setVisible(true);
}

void Joystick::updateJoystick(Touch* touch)
{
	//����ҡ��״̬
	//�����������ж�
	Vec2 hit = touch->getLocation();
	float distance = start.getDistance(hit);
	Vec2 direction = (hit - start).getNormalized();
	//Ϊ�˷�ֹҡ���Ƴ�ҡ�˱���
	if(distance < m_pJoystickr/2)
	{
		m_pJoystick->setPosition(start + (direction * distance));
	}else if(distance >m_pJoystickr) {
		m_pJoystick->setPosition(start + (direction * m_pJoystickr));
	}else {
		m_pJoystick->setPosition(start + (direction * m_pJoystickr/2));
	}

	global->hero->onMove(direction,distance);
}


void Joystick::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
	//�����¼�����
	std::vector<Touch*>::const_iterator touchIter = touches.begin();
	Touch* touch = (Touch*)(*touchIter);
	if(m_pJoystick->getBoundingBox().containsPoint(touch->getLocation()))
	{
		this->showJoystick();
		updateJoystick(touch);
		CCLOG("***************");
		CCLOG("update touch:%f %f",touch->getLocation().x,touch->getLocation().y);

		return;
	}
}

void Joystick::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
	//�ƶ�ʱ����
	std::vector<Touch*>::const_iterator touchIter = touches.begin();
	Touch* touch = (Touch*)(*touchIter);
	if(m_pJoystick->isVisible())
	{
		updateJoystick(touch);
		return;
	}
}

void Joystick::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
	//�뿪�Ǵ���
	//m_pJoystick->runAction(MoveTo::create(0.08f,start));
	//m_pJoystick->setPosition(start);
	global->hero->onStop();
	this->hideJoystick();

}
void Joystick::setJoystick(Vec2 point)
{
	//�����ҡ�˵ķ���ĳ��������
	start =point;
	m_pJoystickBg->setPosition(start);
	m_pJoystick->setPosition(m_pJoystickBg->getPosition());
}
