#ifndef _PLATFORM_OFFICIAL_PLATFORM_CENTER_H_
#define _PLATFORM_OFFICIAL_PLATFORM_CENTER_H_

#include "BasePlatformCenter.h"

class OfficialPlatformCenter : public BasePlatformCenter
{
public:
    OfficialPlatformCenter();
    virtual ~OfficialPlatformCenter();

public:
	//////////////////////////////////////////////////////////////////////////
	// �����¼����
	virtual void goLoginScene();
};

#endif // _PLATFORM_OFFICIAL_PLATFORM_CENTER_H_