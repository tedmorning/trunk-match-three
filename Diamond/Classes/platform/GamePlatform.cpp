#include "GamePlatform.h"
#include "CCPlatformDefine.h"
#include "cocos2d.h"

#ifdef PLATFORM_OFFICIAL
#include "OfficialPlatformCenter.h"
#else
#endif

GamePlatform::GamePlatform()
: m_pPlatform(NULL)
{
}

GamePlatform::~GamePlatform()
{
	CC_SAFE_DELETE(m_pPlatform);
}

GamePlatform& GamePlatform::getInstance()
{
	static GamePlatform platform;
	platform.init();
	return platform;
}

void GamePlatform::goLoginScene()
{
	if (m_pPlatform)
	{
		m_pPlatform->goLoginScene();
	}
}

void GamePlatform::init()
{
	static bool bInit = false;
	if (bInit)
	{
		return;
	}
	bInit = true;

#ifdef PLATFORM_OFFICIAL
	m_pPlatform = new OfficialPlatformCenter();
#else
	cocos2d::CCMessageBox("platform is not define", "");
#endif
}