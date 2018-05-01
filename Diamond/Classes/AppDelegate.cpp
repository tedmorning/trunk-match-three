#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "GamePlatform.h"
#include "AppMacros.h"
#include "DataPool.h"
#include "LoadHelper.h"
#include "SimpleAudioEngine.h"
#include "SoundManager.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    //SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

	pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);

	CCSize frameSize = pEGLView->getFrameSize();
    
    std::vector<std::string> searchPath;

    if (frameSize.height > lowResource.size.height)
	{
		searchPath.push_back(hightResource.directory);

        pDirector->setContentScaleFactor(MIN(hightResource.size.height/designResolutionSize.height, hightResource.size.width/designResolutionSize.width));
	}
	else
    {
        searchPath.push_back(lowResource.directory);

        pDirector->setContentScaleFactor(MIN(lowResource.size.height/designResolutionSize.height, lowResource.size.width/designResolutionSize.width));
    }
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
	
    // pDirector->setDisplayStats(true);
	pDirector->setDisplayStats(false);

    pDirector->setAnimationInterval(1.0 / 60);

	// ¼ÓÔØÊý¾Ý
	LoadHelper::load();

	GamePlatform::getInstance().goLoginScene();

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

	getUserStoreData().saveUserData();

    SoundManager::shareSoundManager().enterBackground();
}

void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SoundManager::shareSoundManager().enterForeground();
}
