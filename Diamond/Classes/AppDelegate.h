#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();
	
    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
	
	//@brief实现CCDirector和CCScene init代码在这里。
	// @return真实初始化成功,程序继续。
	// @return假初始化失败,程序终止。
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
	// @brief函数被称为当应用程序输入的背景
	// @param指针的应用程序
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
	@brief函数被称为当应用程序进入前台
	@param指针的应用程序
    */
    virtual void applicationWillEnterForeground();
};

#endif  // __APP_DELEGATE_H__

