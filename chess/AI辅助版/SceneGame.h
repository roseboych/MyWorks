
#ifndef SCENEGAME_H
#define SCENEGAME_H

#include "cocos2d.h"
USING_NS_CC;
#include "Stone.h"

class Step : public CCObject
{
public:
	int _moveid;
	int _killid;
	int _xFrom;
	int _yFrom;
	int _xTo;
	int _yTo;
	static Step* create(int moveid, int killid, int xFrom, int yFrom, int xTo, int yTo)
	{
		Step* step = new Step;
		step->_killid = killid;
		step->_moveid = moveid;
		step->_xFrom = xFrom;
		step->_xTo = xTo;
		step->_yFrom = yFrom;
		step->_yTo = yTo;
		step->autorelease();
		return step;
	}
};

class SceneGame : public CCLayer
{
public:
	//SceneGame();
	~SceneGame()
	{
		_steps->release();
	}

	static CCScene* scene(bool red);
	static SceneGame* create(bool red);
	bool init(bool red);

	CCPoint _plateOffset;
	CCPoint _stoneOffset; /* _s[0] position */
	float _d; /*  */
	Stone* _s[32];
	bool _redTurn;
	bool _redSide;
	CCArray* _steps;

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	int _selectid;
	bool getClickPos(CCPoint ptInWin, int& x, int &y);
	int getStone(int x, int y);
	void setSelectId(int id);
	CCSprite* _selectSprite;
	void moveStone(int moveid, int killid, int x, int y);
	CCPoint getStonePos(int x, int y);

	void setRealPos(Stone* s);

	void moveComplete(CCNode*, void*);

	bool canMove(int moveid, int killid, int x, int y);
	
	bool canMoveJiang(int moveid, int killid, int x, int y);
	bool canMovePao(int moveid, int killid, int x, int y);
	bool canMoveMa(int moveid, int x, int y);
	bool canMoveShi(int moveid, int x, int y);
	bool canMoveBing(int moveid, int x, int y);
//	bool canMoveShi(int moveid, int x, int y);
	bool canMoveXiang(int moveid, int x, int y);
	bool canMoveChe(int moveid, int x, int y);

	/*reurn -1 就没有棋子， 有的话就返回这个路线上棋子的数量*/
	int getStoneCount(int xo, int yo, int x, int y);

	void back(CCObject* );
};

#endif 
// SCENEGAME_H
