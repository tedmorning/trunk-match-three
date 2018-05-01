#include "OfficialPlatformCenter.h"
#include "FightScene.h"
#include "MainScene.h"
//#include "LoginScene.h"

#ifdef PLATFORM_OFFICIAL

#include "cocos2d.h"
USING_NS_CC;

OfficialPlatformCenter::OfficialPlatformCenter()
{
}

OfficialPlatformCenter::~OfficialPlatformCenter()
{
}

void OfficialPlatformCenter::goLoginScene()
{
	CCScene* scene = MainScene::scene();
	CCDirector::sharedDirector()->runWithScene(scene);
}

#endif