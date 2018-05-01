#ifndef _DATAPOOL_LOAD_HELPER_H_
#define _DATAPOOL_LOAD_HELPER_H_

#include <string>

// ���ؾ�̬���ݼ�����
class LoadHelper
{
public:
	LoadHelper();
	~LoadHelper();

public:
	static void load();

private:
	// ���ر��ؾ�̬����
	static void loadBaseData(std::string file_name);

	// �����ַ����ñ�����
	static void loadBaseText(std::string file_name);
};

#endif // _DATAPOOL_LOAD_HELPER_H_
