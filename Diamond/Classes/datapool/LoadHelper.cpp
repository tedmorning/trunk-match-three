#include "LoadHelper.h"
#include "cocos2d.h"
#include "DataPool.h"

USING_NS_CC;

// ��̬�����Դ�ļ���
#define RES_BASE_DATA    "basedata.xml"

// �ַ����ñ����Դ�ļ���
#define RES_BASE_TEXT    "basetext.xml"

LoadHelper::LoadHelper()
{
}

LoadHelper::~LoadHelper()
{
}

void LoadHelper::load()
{
	loadBaseData(RES_BASE_DATA);
	loadBaseText(RES_BASE_TEXT);
}

void LoadHelper::loadBaseData(std::string file_name)
{
	// ���ر��ؾ�̬����
	unsigned long size;
	unsigned char* base_data = CCFileUtils::sharedFileUtils()->getFileData(file_name.c_str(), "rb", &size);

	DataPool::shareInstance().loadFromXml((const char*)base_data);
	CC_SAFE_DELETE_ARRAY(base_data);
}

void LoadHelper::loadBaseText(std::string file_name)
{
	unsigned long size;
	unsigned char* base_data = CCFileUtils::sharedFileUtils()->getFileData(file_name.c_str(), "rb", &size);

	DataPool::shareInstance().loadFromTextData((const char*)base_data);
	CC_SAFE_DELETE_ARRAY(base_data);
}