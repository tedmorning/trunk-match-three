#ifndef _DATAPOOL_MAP_DATA_H_
#define _DATAPOOL_MAP_DATA_H_

#include "ConfigMap.h"
#include "CopyData.h"

class MapData
{
public:
	MapData(const ConfigMapItem* pCfgMapItem);
	~MapData();

public:
	void initCopyData();

public:
	bool isExistPrevMap() const;
	bool isAllCopyUnlocked() const;

	u32 getId() const;
	u32 getCopyCount() const;
	u32 getPrevId() const;

	std::string getName() const;
	std::string getDesp() const;
	std::string getTexture() const;

	std::string getCopyTexture() const;

	std::vector<CopyData*> getCopyDataList() const;

private:
	const ConfigMapItem* m_pCfgMapItem; // �����ͼ�ṹ����� 
	std::vector<CopyData*> m_vCopyData; // ��Ӧ���ڲ��������� 
};

#endif // _DATAPOOL_MAP_DATA_H_