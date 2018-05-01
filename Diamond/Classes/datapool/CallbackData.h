#ifndef _CALLBACK_ACTION_CALLABACK_H_
#define _CALLBACK_ACTION_CALLABACK_H_

#include "cocos2d.h"

USING_NS_CC;

class CallbackData
{
public:
	CallbackData() : data1(NULL), data2(NULL), data3(NULL), data4(NULL) , data5(NULL), data6(NULL), data7(NULL){}
	~CallbackData() {}

public:
	void* data1;
	void* data2;
	void* data3;
	void* data4;
	void* data5;
	void* data6;
	void* data7;
};

#endif // _CALLBACK_ACTION_CALLABACK_H_