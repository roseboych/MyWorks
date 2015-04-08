#include "Stone.h"

Stone::InitPos Stone::_initPos[16] = {

	{ 0, 0, Stone::CHE },
	{ 1, 0, Stone::MA },
	{ 2, 0, Stone::XIANG },
	{ 3, 0, Stone::SHI },
	{ 4, 0, Stone::JIANG },
	{ 5, 0, Stone::SHI },
	{ 6, 0, Stone::XIANG },
	{ 7, 0, Stone::MA },
	{ 8, 0, Stone::CHE },

	{ 1, 2, Stone::PAO },
	{ 7, 2, Stone::PAO },

	{ 0, 3, Stone::BING },
	{ 2, 3, Stone::BING },
	{ 4, 3, Stone::BING },
	{ 6, 3, Stone::BING },
	{ 8, 3, Stone::BING }
};
Stone* Stone::create(int id,bool red)
{
	Stone* s = new Stone();
	s->init(id,red);
	s->autorelease();
	return s;
}
bool Stone::init(int id, bool red)
{
	CCSprite::init();
	_id = id;
	_red = _id < 16;
	//	int type = 0;
	if (id < 16)
	{
		_type = _initPos[_id]._type;
	}
	else
	{
		_type = _initPos[_id - 16]._type;
	}
	//
	const char* stonePic[14] = {
		"bk.png",//jiang
		"ba.png", //shi
		"bb.png",//xiang
		"br.png",//che
		"bn.png",//ma
		"bc.png",//pao
		"bp.png",//zu
		"rk.png",
		"ra.png",
		"rb.png",
		"rr.png",
		"rn.png",
		"rc.png",
		"rp.png",
	};
	int idx = (_red ? 0 : 1) * 7 + _type;
	CCSprite::initWithFile(stonePic[idx]);

	reset(red);
	return true;
}

void Stone::reset(bool red)//¾­³£
{
	this->setDead(false);
	if (red)
	{
		if (_id < 16)
		{
			this->setX(_initPos[_id]._x);
			this->setY(_initPos[_id]._y);
		}
		else
		{
			this->setX(8 - _initPos[_id - 16]._x);
			this->setY(9 - _initPos[_id - 16]._y);
		}
	}
	else
	{
		if (_id < 16)
		{
			this->setX(8-_initPos[_id]._x);
			this->setY(9-_initPos[_id]._y);
		}
		else
		{
			this->setX(_initPos[_id - 16]._x);
			this->setY(_initPos[_id - 16]._y);
		}
	}
}
