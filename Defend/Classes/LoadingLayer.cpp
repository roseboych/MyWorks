#include "LoadingLayer.h"
#include "WelComeGameLayer.h"
USING_NS_CC;
LoadingLayer::LoadingLayer(){
	this->loadingNum=0;
	this->totalNum=51;
}
bool LoadingLayer::init(){
	bool isRet=false;
	do 
	{
	CC_BREAK_IF(!BaseLayer::init());
	CC_BREAK_IF(!this->setUpdateView());
	
	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/welcomebg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 背景图片
	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/coder.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 添加关于开发者背景图片
  
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/button_sound_on.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 声音开始
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/button_sound_off.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 声音关闭

	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/coder_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 开发者按钮
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/coder_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 开发者按钮

	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/return_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 返回菜单按钮
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/return_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 返回菜单按钮


	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/star_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 开始菜单按钮
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/star_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面  


	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/gamebg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 欢迎界面 背景图片

    CCTextureCache::sharedTextureCache()->addImageAsync("game/weapon.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 弓弩武器
	CCTextureCache::sharedTextureCache()->addImageAsync("game/wq0.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  弓箭子弹


	CCTextureCache::sharedTextureCache()->addImageAsync("game/monster_blood.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 血条背景
	CCTextureCache::sharedTextureCache()->addImageAsync("game/monster_blood_frame.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  血条


	CCTextureCache::sharedTextureCache()->addImageAsync("monster/dutu.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 赌徒怪物

	CCTextureCache::sharedTextureCache()->addImageAsync("game/zcblood.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 主城血条
	CCTextureCache::sharedTextureCache()->addImageAsync("game/magic.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  主城魔法条
	CCTextureCache::sharedTextureCache()->addImageAsync("game/panelblood.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  主城血条和魔法条的背景



	CCTextureCache::sharedTextureCache()->addImageAsync("game/jnl.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 魔法阵背景图片
	CCTextureCache::sharedTextureCache()->addImageAsync("game/jnzt.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 魔法阵CD 亮的图片


	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/pause_button.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏暂停按钮

	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_home_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏家按钮
    CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_home_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); 
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_resume_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏继续按钮
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_resume_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); 
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_retry_down.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏重新开始按钮
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/btn_rety_up.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏暂停按钮


	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/pause_bg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图是游戏暂停按钮


	CCTextureCache::sharedTextureCache()->addImageAsync("game/MagicMatrix.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图魔法阵的图片

	CCTextureCache::sharedTextureCache()->addImageAsync("specia/ligtht.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图闪电特效图片
	CCTextureCache::sharedTextureCache()->addImageAsync("specia/diyu.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图地狱石块特效图片
	CCTextureCache::sharedTextureCache()->addImageAsync("specia/long.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图二龙戏珠特效图片
	CCTextureCache::sharedTextureCache()->addImageAsync("specia/thumbnails.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图特效缩略图片
    

	
	CCTextureCache::sharedTextureCache()->addImageAsync("game/woniubj.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图地蜗牛进度条背景图片
	CCTextureCache::sharedTextureCache()->addImageAsync("game/woniujd.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 此图蜗牛进度条图片
	CCTextureCache::sharedTextureCache()->addImageAsync("game/woniu.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图蜗牛图片

	CCTextureCache::sharedTextureCache()->addImageAsync("game/stage_title.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图等级的背景图


	CCTextureCache::sharedTextureCache()->addImageAsync("game/gameover.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图游戏结束标题图
	CCTextureCache::sharedTextureCache()->addImageAsync("game/gameovertips.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图游戏结束提示图
	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/gameoverbg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图游戏结背景图

	CCTextureCache::sharedTextureCache()->addImageAsync("game/coin.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  此图金币图标

	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/stats_bg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  胜利界面背景图片

	CCTextureCache::sharedTextureCache()->addImageAsync("game/statstip.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  胜利界面提示按钮

	CCTextureCache::sharedTextureCache()->addImageAsync("gmbg/research_bg.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  能力提升界面的背景图片
	CCTextureCache::sharedTextureCache()->addImageAsync("game/lvSyspng.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  能力提升界面的按钮图片
	CCTextureCache::sharedTextureCache()->addImageAsync("game/lvinfo.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  能力提升界面的说明文字图片



	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/outdown.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); //  游戏结束按钮图片
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/outup.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); 
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/updwon.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); // 升级系统图片
	CCTextureCache::sharedTextureCache()->addImageAsync("gmme/upup.png",this,callfuncO_selector(LoadingLayer::loadCallBack)); 

	isRet=true;
	} while (0);
	return isRet;
}
CCScene* LoadingLayer::scene(){
	CCScene* scene=CCScene::create();
	LoadingLayer* layer=LoadingLayer::create();
	scene->addChild(layer);
	return scene;
}
void LoadingLayer::loadCallBack(CCObject* ped){
	loadingNum++;	 
	CCLOG("%d",loadingNum);
    CCProgressTimer* pt=(CCProgressTimer*)this->getChildByTag(1);
	float now=pt->getPercentage();
	pt->setPercentage(100.0/totalNum+now);
	if(loadingNum<totalNum){
		
	}else{
		pt->setPercentage(100);
		// 加载完的时候跳转到响应的界面
		CCLOG("loading over");
		
		goWelcomeLayer();
	}
}


void LoadingLayer::goWelcomeLayer(){
	CCScene* se=WelComeGameLayer::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInR::create(1,se));
}

bool LoadingLayer::setUpdateView(){
	bool isRet=false;
	do 
	{
	// 设置进度条的背景图片 我们把他放到屏幕下方的1/5处	
    CCSprite* loadbackimg=CCSprite::create("gmbg/lodingbg.png");
	CC_BREAK_IF(!loadbackimg);	
	loadbackimg->setPosition(ccp(getWinSize().width/2+getWinOrigin().x,getWinSize().height/5+getWinOrigin().y));
	this->addChild(loadbackimg,1);
	
	// 添加进度条
	CCSprite* loadimg=CCSprite::create("gmbg/longding.png");
	CC_BREAK_IF(!loadimg);	
	CCProgressTimer* pt = CCProgressTimer::create(loadimg);
	pt->setType(kCCProgressTimerTypeBar);// 设置成横向的
	//可以看作是按矩形显示效果的进度条类型
	pt->setMidpoint(ccp(0,0)); 
	//  用来设定进度条横向前进的方向从左向右或是从右向左
	pt->setBarChangeRate(ccp(1,0));
	//重新设置锚点
	float tex=getWinSize().width/2+getWinOrigin().x;
	float tey=getWinSize().height/5+getWinOrigin().y-5;
	pt->setPosition(ccp(tex,tey));
	pt->setPercentage(0);
	this->addChild(pt,2,1);

	isRet=true;
	} while (0);
	return isRet;
}