#include "LoginScene.h"

USING_NS_CC;

CCScene* LoginScene::scene()
{
    CCScene* scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        LoginScene *layer = LoginScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool LoginScene::init()
{
	if (CCLayer::init())
	{
		return true;
	}
	return false;
}

void LoginScene::onCombo(int combo)
{
	CCLog("combo: %d", combo);
}

void LoginScene::onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem)
{
	CCLog("explode");
}