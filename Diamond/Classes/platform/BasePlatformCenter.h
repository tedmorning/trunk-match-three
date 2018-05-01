#ifndef _PLATFORM_BASE_PLATFORM_CENTER_H_
#define _PLATFORM_BASE_PLATFORM_CENTER_H_

#include "PlatformDefine.h"

class BasePlatformCenter
{
public:
    BasePlatformCenter();
    virtual ~BasePlatformCenter();

public:
	//////////////////////////////////////////////////////////////////////////
	// 进入登录界面
	virtual void goLoginScene() = 0;

	// 进入用户支付页面
	virtual void goPayScene() {}

	// 进入用户中心
	virtual void goUserCenter() {}

	//////////////////////////////////////////////////////////////////////////
	// 登录
	// 注册
	virtual void login() {}
	virtual void logout() {}

	// 购买商品
	virtual void buy() {}

	// 判断用户是否已经登录
	virtual bool isLogined() { return true; }

	//////////////////////////////////////////////////////////////////////////
	// 获取用户信息
	//virtual std::string getName() = 0;
	//virtual std::string getSession() = 0;
};

#endif // _PLATFORM_BASE_PLATFORM_CENTER_H_