#include "MapLayer.h"

MapLayer::MapLayer()
{
}
MapLayer::~MapLayer()
{
}

bool MapLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Layer::init() );
		this->initMapWithFile("mymap.tmx");//��ͼ��ʼ��

		this->scheduleUpdate();
		ret = true;
	} while(0);

	return ret;
}
void MapLayer::initMapWithFile(const char * path)
{
	TMXTiledMap *TileMap = TMXTiledMap::create(path);
	TileMap->setPosition(Vec2(0,0));
	global->tileMap=TileMap;
	this->addChild(TileMap); 
}

void MapLayer::update(float dt)
{
	this->setViewpointCenter(global->hero->getPosition());
}

void MapLayer::setViewpointCenter(Point pos)  //������ƶ���ͼ,ͬʱ����X,Y���׼�㷨
{  
	Size winSize = Director::getInstance()->getWinSize();  
	auto _map = global->tileMap;

	//�����������С����Ļ��һ�룬��ȡ��Ļ�е����꣬����ȡ���������  
	int x = MAX(pos.x, winSize.width/2);  
	int y = MAX(pos.y, winSize.height/2);  

	//���X��Y������������Ͻǵļ���ֵ����ȡ����ֵ�����꣨����ֵ��ָ���õ�ͼ������Ļ��ɳ��ֺڱߵļ������� )
	x = MIN(x, (_map->getMapSize().width * _map->getTileSize().width) - winSize.width/2);  
	y = MIN(y, (_map->getMapSize().height * _map->getTileSize().height) - winSize.height/2);  

	//����ǰ��������
	Point actualPosition = Vec2(x, y); 

	//������Ļ�е����Ҫ�ƶ���Ŀ�ĵ�֮��ľ���
	Point centerOfView = Vec2(winSize.width/2, winSize.height/2);  
	Point viewPoint = centerOfView - actualPosition;  

	//�趨һ�µ�ͼ��λ�� ,����һ��Ҫע��,�����ƶ��Լ�������_MAP�ƶ�������Ч��,���������㿨�˺ó�ʱ��
	//_map->setPosition(viewPoint);  
	//this->setPosition(viewPoint);
	//global->gameLayer->setPosition(viewPoint);
	this->getParent()->setPosition(viewPoint);
}  
