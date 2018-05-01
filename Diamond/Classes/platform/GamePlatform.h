#ifndef _PLATFORM_GAME_PLATFORM_H_
#define _PLATFORM_GAME_PLATFORM_H_

#include "BasePlatformCenter.h"

class BasePlatformCenter;

class GamePlatform
{
public:
    virtual ~GamePlatform();

    static GamePlatform& getInstance();

public:
    BasePlatformCenter* getPlatformCenter();

	// 进入登陆界面
	// 每个平台的登陆界面不一样
	void goLoginScene();

private:
    GamePlatform();

    void init();

private:
	BasePlatformCenter* m_pPlatform;
};

#endif // _PLATFORM_GAME_PLATFORM_H_
