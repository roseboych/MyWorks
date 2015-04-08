#ifndef __ScenceStart_H__
#define __ScenceStart_H__

#include "cocos2d.h"
USING_NS_CC;

class ScenceStart : public cocos2d::CCLayer
{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	CREATE_FUNC(ScenceStart);

	CCSprite* red;
	CCSprite* black;
	bool isSlecet;

	void update(float delta);

	
};

#endif // __HELLOWORLD_SCENE_H__
