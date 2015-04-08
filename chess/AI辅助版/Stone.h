#ifndef _STONE_H_
#define _STONE_H_
#include "cocos2d.h"
USING_NS_CC;



class Stone : public cocos2d::CCSprite
{
public:
	enum TYPE
	{
		JIANG, SHI, XIANG, CHE, MA, PAO, BING
	};
	static struct InitPos
	{
		int _x;
		int _y;
		Stone::TYPE _type;
	}_initPos[16];

	CC_SYNTHESIZE(int, x, X);
	CC_SYNTHESIZE(int, y, Y);

	CC_SYNTHESIZE(int, _id, ID);
	CC_SYNTHESIZE(bool, _dead, Dead);
	CC_SYNTHESIZE(bool, _red, Red);
	CC_SYNTHESIZE(TYPE, _type, Type);


	static Stone* create(int id, bool red);
	bool init(int id, bool red);
	void reset(bool red);
};

#endif
