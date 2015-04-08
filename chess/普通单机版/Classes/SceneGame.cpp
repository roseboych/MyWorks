
#include "SceneGame.h"

CCScene* SceneGame::scene(bool red)
{
	CCScene *scene = CCScene::create();
	SceneGame *layer = SceneGame::create(red);
	scene->addChild(layer);
	return scene;
}

SceneGame* SceneGame::create(bool red)
{
	SceneGame* pRet = new SceneGame();
	if (pRet)
	{
		if (pRet->init(red))
		{
			pRet->autorelease();
		}

		else
		{
			pRet->release();
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
	return pRet;
}

bool SceneGame::init(bool red)
{
	if (!CCLayer::init())
	{
		return false;
	}
	_redSide = red;
	this->_plateOffset = ccp(20, 10);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	
	_redTurn = true;

	this->_stoneOffset = ccp(59, 38);
	this->_d = 46;
	//add desk
	CCSprite* desk = CCSprite::create("floor.jpg");
	this->addChild(desk);
	desk->setPosition(ccp(size.width / 2, size.height / 2));
	desk->setScaleX(size.width / desk->getContentSize().width);
	desk->setScaleY(size.height / desk->getContentSize().height);


	//add plane ,stone;
	CCSprite* plate = CCSprite::create("background.png");
	this->addChild(plate);
	plate->setAnchorPoint(CCPointZero);
	plate->setPosition(_plateOffset);
	plate->setScale((size.height-_plateOffset.y*2)/ plate->getContentSize().height);

	//add selceed  sprite;
	_selectid = -1;
	_selectSprite = CCSprite::create("selected.png");
	addChild(_selectSprite);
	_selectSprite->setVisible(false);
	_selectSprite->setZOrder(1000);
	_selectSprite->setScale(.8f);
	
	//add stone

	for (int i = 0; i < 32; i++)
	{
		_s[i] = Stone::create(i,red);
		addChild(_s[i]);  
	//	_s[i]->setPosition(ccp(_s[i]->getX() * _d, _s[i]->getY()*_d) + _stoneOffset);
	//	setRealPos(_s[i]);
		_s[i]->setPosition(ccp(CCRANDOM_0_1()*size.width, CCRANDOM_0_1()*size.height));
		CCMoveTo* move = CCMoveTo::create(1, this->getStonePos(_s[i]->getX(), _s[i]->getY()));
		_s[i]->runAction(move);
	//	_s[i]->setScale(0.9f);
	}
	//add touch
	setTouchEnabled(true);
	setTouchMode(ccTouchesMode::kCCTouchesOneByOne);


	//huiqi
	CCMenu* pMenu = CCMenu::create();
	//CCMenuItemFont * pItem = CCMenuItemFont::create("HUIQI", this, menu_selector(SceneGame::back));
	CCMenuItem* pItem = CCMenuItemImage::create("regret.jpg","regret.jpg",this,menu_selector(SceneGame::back) );
	pMenu->addChild(pItem);
	addChild(pMenu);
	pMenu->setPositionX(pItem->getPositionX() + 650);
		//
	_steps = CCArray::create();
	_steps->retain();//转换成非自动管理 在析构的时候释放；

	return true;
}


void SceneGame::setSelectId(int id)
{
	if (id == -1)
		return;

	if (_s[id]->getRed() == _redTurn)
		return;

	_selectid = id;

	_selectSprite->setVisible(true);
	_selectSprite->setPosition(_s[_selectid]->getPosition());
}

bool SceneGame::ccTouchBegan(CCTouch *pTouch, CCEvent *)
{
	CCPoint ptInWin = pTouch->getLocation();

	int x, y;
	if (!getClickPos(ptInWin, x, y))
	{
		return false;
	}

	CCLog("x=%d, y=%d", x, y);

	int clickid = getStone(x, y);
	CCLog("clickid=%d", clickid);
	if (_selectid == -1)
	{
		setSelectId(clickid);
	}
	else
	{
		moveStone(_selectid, clickid, x, y);
	}


	return true;
}

int SceneGame::getStone(int x, int y)
{
	Stone* s;
	for (int i = 0; i < 32; i++)
	{
		s = _s[i];
		if (s->getX() == x && s->getY() == y && !s->getDead())
			return s->getID();
	}

	return -1;
}

bool SceneGame::getClickPos(CCPoint ptInWin, int &x, int &y)
{
	for (x = 0; x <= 8; x++)
	for (y = 0; y <= 9; y++)
	{
		CCPoint ptInPlate = ccp(x*_d, y*_d) + _stoneOffset;
		if (ptInWin.getDistance(ptInPlate) < _d / 2)
		{
			return true;
		}
	}
	return false;
}

void SceneGame::moveStone(int moveid, int killid, int x, int y)
{
	if (killid != -1 && _s[moveid]->getRed() == _s[killid]->getRed())
	{
		setSelectId(killid);
		return;
	}



	bool bCanMove = canMove(moveid, killid, x, y);
	if (bCanMove == false)
		return;

	// set Step
	Step* step = Step::create(moveid, killid, _s[moveid]->getX(), _s[moveid]->getY(), x, y);
	_steps->addObject(step);

	_s[moveid]->setX(x);
	_s[moveid]->setY(y);
	//  _s[moveid]->setPosition( getStonePos(x, y) );
	//   setRealPos(_s[moveid]);
	CCMoveTo* move = CCMoveTo::create(.5f, getStonePos(x, y));
	CCCallFuncND* call = CCCallFuncND::create(this,
		callfuncND_selector(SceneGame::moveComplete),
		(void*)(intptr_t)killid);
	CCSequence* seq = CCSequence::create(move, call, NULL);
	_s[moveid]->setZOrder(_s[moveid]->getZOrder() + 1);
	_s[moveid]->runAction(seq);
}
void SceneGame::moveComplete(CCNode* moveStone, void* _killid)
{
	moveStone->setZOrder(moveStone->getZOrder() - 1);
	int killid = (int)(intptr_t)_killid;
	if (killid != -1)
	{
		_s[killid]->setDead(true);
		_s[killid]->setVisible(false);

		if (_s[killid]->getType() == Stone::JIANG)
		{
			CCDirector::sharedDirector()->replaceScene(SceneGame::scene(!_redSide));
		}
	}
	_selectid = -1;
	_selectSprite->setVisible(false);
	_redTurn = !_redTurn;
}


CCPoint SceneGame::getStonePos(int x, int y)
{
	int xx = x* _d;
	int yy = y*_d;
	return ccp(xx, yy) + _stoneOffset;
}
void SceneGame::back(CCObject*  pObect)
{
	if (_steps->count() == 0)
	{
		return;
	}
	Step* step = (Step*)_steps->lastObject();
	_s[step->_moveid]->setX(step->_xFrom);
	_s[step->_moveid]->setY(step->_yFrom);

	_s[step->_moveid]->setPosition(getStonePos(step->_xFrom, step->_yFrom));

	if (step->_killid != -1)//复活
	{
		_s[step->_killid]->setVisible(true);
		_s[step->_killid]->setDead(false);
	}
	_redTurn = !_redTurn;
	_steps->removeLastObject();
}
void SceneGame::setRealPos(Stone* s)
{
	s->setPosition(getStonePos(s->getX(), s->getY()));
}
bool SceneGame::canMove(int moveid, int killid, int x, int y)
{
	Stone* s = _s[moveid];
	switch (s->getType())
	{
	case Stone::JIANG:
		return canMoveJiang(moveid,killid, x, y);
	case Stone::SHI:
		return canMoveShi(moveid, x, y);
	case Stone::BING:
		return canMoveBing(moveid, x, y);
	case Stone::MA:
		return canMoveMa(moveid, x, y);
	case Stone::CHE:
		return canMoveChe(moveid, x, y);
	case Stone::PAO:
		return canMovePao(moveid, killid, x, y);
	case Stone::XIANG:
		return canMoveXiang(moveid, x, y);
	}
	return	false;
}
bool SceneGame::canMoveJiang(int moveid, int killid, int x, int y)
{
	  // 老将对杀 
	Stone* skill = _s[killid];
	if (skill->getType() == Stone::JIANG)
	{
		return canMoveChe(moveid, x, y);
	}
	// 不能出9宫，  每次走一个格 
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	//if (xoff > 1) return false;
	//if (yoff > 1) return false;
	//if (xoff + yoff != 1) return false;
	int d = xoff * 10 + yoff;
	if (d != 1 && d != 10)
		return false;

	if (x<3 || x>5)
	if (_redSide == s->getRed())
	{
		if (y<0 || y>2)
		{
			return false;
		}
	}
	else
	{
		if (y<7 || y>9)
		{
			return false;
		}
	}
	return true;
}
bool SceneGame::canMoveShi(int moveid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	/*if (xoff != 1) return false;
	if (yoff != 1) return false;*/
	int d = xoff * 10 + yoff;
	if ( d != 11) 
	{
		return false;
	}

//	if (xoff + yoff != 1) return false;
	if (x<3 || x>5)
	if (_redSide == s->getRed())
	{
		if (y<0 || y>2)
		{
			return false;
		}
	}
	else
	{
		if (y<7 || y>9)
		{
			return false;
		}
	}
	return true;
}
bool SceneGame::canMoveXiang(int moveid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	/*if (xoff != 2) return false;
	if (yoff != 2) return false;*/
//	if (xoff + yoff != 1) return false;
	int d = xoff * 10 + yoff;
	if (d != 22) return false;

	int xm = (xo + x)/2;
	int ym = (yo + y) / 2;
	int id = getStone(xm, ym);
	if (id != -1) return false;

	if (_redSide == s->getRed())
	{
		if (y > 4) return false;
	}
	else{
		if (y < 5) return false;
	}
	return true;
}
bool SceneGame::canMoveChe(int moveid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();

	if (getStoneCount(xo, yo, x, y) != 0)
	{
		return false;
	}
	return true;
}
bool SceneGame::canMovePao(int moveid, int killid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();

	if (killid != -1 && this->getStoneCount(xo, yo, x, y) == 1) return true;
//	if (this->getStoneCount(xo, yo, x, y) > 1) return false;
	//if (this->getStoneCount(xo, yo, x, y) == 0) return canMoveChe(moveid, x, y);
	//else
	//	return false;
	return canMoveChe(moveid, x, y);
}
//bool SceneGame::canMovePao(int moveid, int killid, int x, int y)
//{
//	Stone* s = _s[moveid];
//	int xo = s->getX();
//	int yo = s->getY();
//
//	if (killid != -1 && this->getStoneCount(xo, yo, x, y) == 1) return true;
//
//	return canMoveChe(moveid, x, y);
//}
bool SceneGame::canMoveBing(int moveid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	if (d != 1 && d != 10)
		return false;
	if (_redSide == s->getRed())
	{
		if (y<yo)//不能后退
			return false;
		if (yo <= 4 && y == yo)// 没过河又想横着走；
			return false;
	}
	else{
		if (y>yo)//不能后退
			return false;
		if (yo >= 5 && y == yo)// 没过河又想横着走；
			return false;
	}
	return true;
}
bool SceneGame::canMoveMa(int moveid, int x, int y)
{
	Stone* s = _s[moveid];
	int xo = s->getX();
	int yo = s->getY();
	int xoff = abs(xo - x);
	int yoff = abs(yo - y);
	int d = xoff * 10 + yoff;
	if (d != 12 && d != 21) return false;

	int xm, ym;
	if (d == 12)
	{
		 xm = xo;
		 ym = (yo + y) / 2;
	}
	else
	{
		xm = (xo + x) / 2;
		ym = yo;
	}
	if (getStone(xm, ym) != -1) return false;

	return true;
}

int SceneGame::getStoneCount(int xo, int yo, int x, int y)
{
	int ret = 0;
	if (xo != x && yo != y) return -1;
	if (xo == x && yo == y) return -1;
	if (xo == x)
	{
		int min = yo < y ? yo : y;
		int max = yo + y - min;
		for (int yy = min + 1; yy < max; yy++)
		{
			if (getStone(xo, yy) != -1) ++ret;
		}
	}
	else{
			int min = xo < x ? xo : x;
			int max = xo + x - min;
			for (int xx = min + 1; xx < max; xx++)
			{
				if (getStone(xx, xo) != -1) ++ret;
			}
	}
	return ret;
}




