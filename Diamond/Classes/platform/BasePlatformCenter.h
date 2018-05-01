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
	// �����¼����
	virtual void goLoginScene() = 0;

	// �����û�֧��ҳ��
	virtual void goPayScene() {}

	// �����û�����
	virtual void goUserCenter() {}

	//////////////////////////////////////////////////////////////////////////
	// ��¼
	// ע��
	virtual void login() {}
	virtual void logout() {}

	// ������Ʒ
	virtual void buy() {}

	// �ж��û��Ƿ��Ѿ���¼
	virtual bool isLogined() { return true; }

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ�û���Ϣ
	//virtual std::string getName() = 0;
	//virtual std::string getSession() = 0;
};

#endif // _PLATFORM_BASE_PLATFORM_CENTER_H_