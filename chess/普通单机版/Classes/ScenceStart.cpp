#include "ScenceStart.h"
#include "SceneGame.h"


CCScene* ScenceStart::scene()//ok
{
	CCScene* scene = CCScene::create();
	ScenceStart* layer = ScenceStart::create();
	scene->addChild(layer);
	return scene;
}

bool ScenceStart::init()//ok
{
	CCLayer::init();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCSprite* Jiang = CCSprite::create("bkg1.png");
	addChild(Jiang);
	Jiang->setPosition(ccp(100, size.height / 2));
	
	CCSprite* Shuai = CCSprite::create("bkg2.png");
	addChild(Shuai);
	Shuai->setPosition(ccp(size.width-100, size.height / 2));
	black = Jiang;
	  red = Shuai;

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	return true;
}
bool ScenceStart::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)//ok
{
	return true;
}
void ScenceStart::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)//ok
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	bool bClickStone = false;
	CCPoint ptClick = pTouch->getLocation();
	if (red->boundingBox().containsPoint(ptClick))
	{
		this->isSlecet = true;
		bClickStone = true;
	}
	else if (black->boundingBox().containsPoint(ptClick))
	{
		this->isSlecet = false;
		bClickStone = true;
	}
	if (bClickStone)
	{
		//move stone
		CCMoveTo* moveTo1 = CCMoveTo::create(1, ccp(size.width / 2, size.height / 2));
		CCMoveTo* moveTo2 = CCMoveTo::create(1, ccp(size.width / 2, size.height / 2));
		CCRotateBy* Roate1 = CCRotateBy::create(1, -360);
		CCRotateBy* Roate2 = CCRotateBy::create(1, 360);

		CCSpawn* spawn1 = CCSpawn::create(moveTo1, Roate1,NULL);
		CCSpawn* spawn2 = CCSpawn::create(moveTo2, Roate2, NULL);

		red->runAction(spawn1);
		black->runAction(spawn2);

		scheduleUpdate();

	}
}
void ScenceStart::update(float )//ok
{
	float x1 = red->getPositionX();
	float x2 = red->getPositionX();
	if (abs(x1 - x2) < red->getContentSize().width)
	{
		//¿ªÊ¼ÓÎÏ·
		CCDirector::sharedDirector()->replaceScene(SceneGame::scene(this->isSlecet));
	}
}