#ifndef _DATAPOOL_LOAD_HELPER_H_
#define _DATAPOOL_LOAD_HELPER_H_

#include <string>

// 本地静态数据加载类
class LoadHelper
{
public:
	LoadHelper();
	~LoadHelper();

public:
	static void load();

private:
	// 加载本地静态数据
	static void loadBaseData(std::string file_name);

	// 加载字符配置表数据
	static void loadBaseText(std::string file_name);
};

#endif // _DATAPOOL_LOAD_HELPER_H_
