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

	// �����½����
	// ÿ��ƽ̨�ĵ�½���治һ��
	void goLoginScene();

private:
    GamePlatform();

    void init();

private:
	BasePlatformCenter* m_pPlatform;
};

#endif // _PLATFORM_GAME_PLATFORM_H_
