#ifndef _DATAPOOL_PROP_DATA_H_
#define _DATAPOOL_PROP_DATA_H_

#include "ConfigProp.h"

class PropData
{
public:
	PropData(int nPropId, int nPropNum);
	PropData(const ConfigPropItem* nCfgPropItem);
	~PropData();

	bool enableUse();
	int getEnablePropNum();
	void addUsedPropNum(int num);

	int getId();
	std::string getTexture();
	std::string getName();
	std::string getDesp();
	int getParam();

private:
	int m_nUsedPropNum;
	int m_nPropNum;
	const ConfigPropItem* m_pCfgPropItem;
};

#endif // _DATAPOOL_PROP_DATA_H_