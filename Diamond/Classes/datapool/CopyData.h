#ifndef _DATAPOOL_COPY_DATA_H_
#define _DATAPOOL_COPY_DATA_H_

#include "ConfigCopy.h"
#include "MarcoDefine.h"

class CopyData
{
public:
	CopyData(const ConfigCopyItem* pCfgCopyItem);
	~CopyData();

	bool isLocked() const;

	u32 getScore() const;

	u32 getId() const;
	u32 getMapId() const;
	u32 getWaveCount() const;
	u32 getMonsterCount() const;
	u32 getTime() const;
	u32 getHp() const;

	std::string getName() const;
	std::string getDesp() const;
	std::string getTexture() const;
	int getcopyButtonX();
	int getcopyButtonY();

	bool getMineralValue();
	// 是否为无限模式 
	bool isInfinite() const;

private:
	const ConfigCopyItem* m_pCfgCopyItem;
};

#endif // _DATAPOOL_COPY_DATA_H_