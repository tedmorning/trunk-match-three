#ifndef _DATAPOOL_H_
#define _DATAPOOL_H_

#include "ConfigMap.h"
#include "ConfigCopy.h"
#include "ConfigMonster.h"
#include "ConfigMonsterSkill.h"
#include "ConfigCopyWave.h"
#include "ConfigWaveMonster.h"
#include "ConfigSpriteSkill.h"
#include "ConfigSpriteSkillStatus.h"
#include "ConfigStatusType.h"
#include "ConfigText.h"
#include "ConfigProp.h"
#include "ConfigCopyProp.h"
#include "ConfigCopySkill.h"
#include "ConfigStore.h"

#include "UserData.h"
#include "MapDataManager.h"
#include "ConfigConst.h"


// 数据池 - 用于管理本地数据
class DataPool
{
private:
	DataPool();
	~DataPool();

public:
	static DataPool& shareInstance();

	// 加载静态表数据
	void loadFromXml(const char* file_data);

	// 加载字符配置表
	void loadFromTextData(const char* file_data);

	////////////////////////////////////////////////////////////////////////////////////
	// 读取数据
	
	// 副本信息
	const ConfigMap& getConfigMap() const { return m_ConfigMap; }
	const ConfigCopy& getConfigCopy() const { return m_ConfigCopy; }
	const ConfigCopyWave& getConfigCopyWave() const { return m_ConfigCopyWave; }
	ConfigWaveMonster& getConfigWaveMonster() { return m_ConfigWaveMonster; }
	const ConfigCopySkill& getConfigCopySkill() const { return m_ConfigCopySkill; }

	// 怪物
	const ConfigMonster& getConfigMonster() const { return m_ConfigMonster; }

	// 怪物技能
	const ConfigMonsterSkill& getConfigMonsterSkill() const { return m_ConfigMonsterSkill; }

	// 精灵技能
	const ConfigSpriteSkill& getConfigSpriteSkill() const { return m_ConfigSpriteSkill; }
	const ConfigSpriteSkillStatus& getConfigSpriteSkillStatus() const { return m_ConfigSpriteSkillStatus; }
	const ConfigStatusType& getConfigStatusType() const { return m_ConfigStatusType; }

	// 字符配置表
	const ConfigText& getConfigText() const { return m_ConfigText; }

	// 道具
	const ConfigProp& getConfigProp() const { return m_ConfigProp; }
	const ConfigCopyProp& getConfigCopyProp() const { return m_ConfigCopyProp; }

	// 商城
	const ConfigStore& getConfigStroe() const { return m_ConfigStore; } 

	// 用户数据
	UserData& getUserStoreData() { return m_UserData; }
	const MapDataManager& getMapDataManager() const { return m_MapDataManager; }

	// 常量
	const ConfigConst& getConst() const { return m_ConfigConst; } 

	// 关于无限模式
	std::vector<ConfigCopyWaveItem*> createCopyWaveForInfiniti(u32 copyId, float time); // 计算无限模式的副本波数表

	std::vector<ConfigWaveMonsterItem* > createWaveMonsterForInfiniti(u32 copyId); // 计算无限模式的波数怪物表

	std::vector<ConfigWaveMonsterItem*> getCurWaveMonsterItem(u32 waveId);

private:
	ConfigMap m_ConfigMap;
	ConfigCopy  m_ConfigCopy;
	ConfigMonster m_ConfigMonster;
	ConfigCopyWave m_ConfigCopyWave;
	ConfigWaveMonster m_ConfigWaveMonster;
	ConfigSpriteSkill m_ConfigSpriteSkill;
	ConfigSpriteSkillStatus m_ConfigSpriteSkillStatus;
	ConfigStatusType m_ConfigStatusType;
	ConfigProp m_ConfigProp;
	ConfigCopyProp m_ConfigCopyProp;
	ConfigCopySkill m_ConfigCopySkill;
	ConfigText m_ConfigText;
	ConfigConst m_ConfigConst;
	ConfigStore  m_ConfigStore; 
	ConfigMonsterSkill m_ConfigMonsterSkill;


	UserData m_UserData;
	MapDataManager m_MapDataManager;

	// 关于无限模式
	std::vector<ConfigWaveMonsterItem*> m_vWaveMonsterForInfiniti;
	std::vector<ConfigCopyWaveItem*>  m_vCopyWaveForInfiniti;
	float m_timeWithMonsterHp;
	int m_nCount;
	int m_nBossCount;
};

#define getConfigMap()                 DataPool::shareInstance().getConfigMap()
#define getConfigCopy()                DataPool::shareInstance().getConfigCopy()
#define getConfigCopyWave()            DataPool::shareInstance().getConfigCopyWave()
#define getConfigWaveMonster()         DataPool::shareInstance().getConfigWaveMonster()
#define getConfigMonster()             DataPool::shareInstance().getConfigMonster()
#define getConfigMonsterSkill()        DataPool::shareInstance().getConfigMonsterSkill()
#define getConfigSpriteSkill()         DataPool::shareInstance().getConfigSpriteSkill()
#define getConfigSpriteSkillStatus()   DataPool::shareInstance().getConfigSpriteSkillStatus()
#define getConfigProp()				   DataPool::shareInstance().getConfigProp()
#define getConfigCopyProp()			   DataPool::shareInstance().getConfigCopyProp()
#define getConfigCopySkill()		   DataPool::shareInstance().getConfigCopySkill()
#define getConfigStore()               DataPool::shareInstance().getConfigStroe()

#define getUserStoreData()			   DataPool::shareInstance().getUserStoreData()
#define getMapDataManager()			   DataPool::shareInstance().getMapDataManager()

#define getConst(id)				   DataPool::shareInstance().getConst().getConst(id)
#define ST_STR(id)				       DataPool::shareInstance().getConfigText().getText(id)

#define getcreateWaveMonsterForInfiniti(copyId)  DataPool::shareInstance().createWaveMonsterForInfiniti(copyId);
#define getcreateCopyWaveForInfiniti(copyId, time)     DataPool::shareInstance().createCopyWaveForInfiniti(copyId, time);
#define getCurWaveMonsterItemWithInfiniti(waveId)      DataPool::shareInstance().getCurWaveMonsterItem(waveId);
#endif // _DATAPOOL_H_
