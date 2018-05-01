#include "DataPool.h"
#include "cocos2d.h"
#include "tinyxml.h"

USING_NS_CC;

DataPool::DataPool()
{
	m_timeWithMonsterHp = 1.2f;
	m_nCount = 0;
	m_nBossCount = 0;
}

DataPool::~DataPool()
{
}

DataPool& DataPool::shareInstance()
{
	static DataPool data_pool;
	return data_pool;
}

void DataPool::loadFromXml(const char* file_data)
{
	if (file_data == NULL)
	{
		return;
	}
	
	TiXmlDocument document;
	document.Parse(file_data);
	TiXmlElement* root_element = document.RootElement();
	if (root_element == NULL)
	{
		CCMessageBox("静态表读取发生错误", "error");
		return;
	}

	TiXmlElement* first_element = document.FirstChildElement("basedata");
	if (first_element == NULL)
	{
		return;
	}
	
	// 解析数据
	m_ConfigConst.loadFromXml(first_element->FirstChildElement("const"));
	m_ConfigMap.loadFromXml(first_element->FirstChildElement("map"));
	m_ConfigCopy.loadFromXml(first_element->FirstChildElement("copy"));
	m_ConfigMonster.loadFromXml(first_element->FirstChildElement("monster"));
	m_ConfigWaveMonster.loadFromXml(first_element->FirstChildElement("wavemonster"));
	m_ConfigCopyWave.loadFromXml(first_element->FirstChildElement("copywave"));
	m_ConfigSpriteSkill.loadFromXml(first_element->FirstChildElement("spriteskill"));
	m_ConfigSpriteSkillStatus.loadFromXml(first_element->FirstChildElement("spriteskillstatus"));
	m_ConfigStatusType.loadFromXml(first_element->FirstChildElement("statustype"));
	m_ConfigProp.loadFromXml(first_element->FirstChildElement("prop"));
	m_ConfigCopyProp.loadFromXml(first_element->FirstChildElement("copyprop"));
	m_ConfigCopySkill.loadFromXml(first_element->FirstChildElement("copyskill"));
	m_ConfigStore.loadFromXml(first_element->FirstChildElement("shop"));
	m_ConfigMonsterSkill.loadFromXml(first_element->FirstChildElement("monsterSkill"));

	// 加载用户数据
	m_UserData.loadUserData();
	m_MapDataManager.initData();
}

void DataPool::loadFromTextData(const char* file_data)
{
	if (file_data == NULL)
	{
		return;
	}
	
	TiXmlDocument document;
	document.Parse(file_data);
	TiXmlElement* root_element = document.RootElement();
	if (root_element == NULL)
	{
		CCMessageBox("字符配置表读取发生错误", "error");
		return;
	}

	TiXmlElement* text_element = document.FirstChildElement("Language");
	if (text_element == NULL)
	{
		return;
	}
	
	// 解析数据
	m_ConfigText.loadFromXml(text_element);
}

// 计算无限模式的波数怪物表 

std::vector<ConfigWaveMonsterItem*> DataPool::createWaveMonsterForInfiniti(u32 copyId)
{
	// std::vector<const ConfigWaveMonsterItem*> m_vWaveMonsterForInfiniti;
	
	if (m_vWaveMonsterForInfiniti.size() > 0)
	{
		m_vWaveMonsterForInfiniti.clear();
	}
	srand(time(0));
	u32 vectorId = 0;
	if (copyId == 25)// 无限模式是第25关
	{
		int count = 0 ; // 用来计数，没多少波出现一只boss
		// m_nBossCount; // 用来计算出哪只boss
		for (u32 i = 0; i < 10; ++i) // 波数id
		{
			++count;
			int pool = (rand()%4+1); // 随机计算这一波的怪有几只
			if ( m_timeWithMonsterHp <= 0.5f && pool <=2) // 增加难度用
			{
				pool = 3;
			}
			else if (m_timeWithMonsterHp <= 1.f && pool == 1)
			{
				pool = 2;
			}
			int  idx1 = -1;
			int  idx2 = -1;
			int  idx3 = -1;
			int  idx4 = -1;
			for (int j = 1; j <= pool; ++j)  // 循环这一波 
			{
				ConfigWaveMonsterItem* monsterItem = new ConfigWaveMonsterItem();
				int  monsterId = rand()%7;  // 随机计算要哪只怪
				int  randRowIdx = rand()%4; // 随机计算怪物出现在第几行（不能重复）
				int  monsterRowIdx = -1;    // 需要使用的怪物idx（while中计算） 
				if (count >= 10)
				{
					pool = 1;
					count = 0;
					if (m_nBossCount == 0)
					{
						 monsterId = 18;
						 ++m_nBossCount;
					}
					else if (m_nBossCount == 1)
					{
						monsterId = 21;
						++m_nBossCount;
					}
					else if (m_nBossCount == 2)
					{
						monsterId = 22;
						++m_nBossCount;
					}
					else if (m_nBossCount == 3)
					{
						monsterId = 23;
						m_nBossCount = 0;
					}
				}
				while (1)  // 去除在同一波中出现相同的行位置 
				{
					if (idx1 == -1)
					{
						idx1 = randRowIdx;
						monsterRowIdx = randRowIdx;
						break;
					}
					if (idx2 == -1 && randRowIdx!= idx1)
					{
						idx2 = randRowIdx;
						monsterRowIdx = randRowIdx;
						break;
					}else{ randRowIdx = rand()%4;}

					if (idx3 == -1 && randRowIdx != idx2 && randRowIdx != idx1)
					{
						idx3 = randRowIdx;
						monsterRowIdx = randRowIdx;
						break;
					}else{ randRowIdx = rand()%4;}

					if (idx4 == -1 && randRowIdx != idx3 && randRowIdx != idx2 && randRowIdx != idx1)
					{
						idx4 = randRowIdx;
						monsterRowIdx = randRowIdx;
						break;
					}
				}
				switch (monsterId)
				{
				case 0:
					if (m_timeWithMonsterHp <= 0.1f)
					{
						monsterId = 19;
					}
					else
					{
						monsterId = 2;
					}	
					break;
				case 1:
					monsterId = 2;
					break;
				case 2:
					monsterId = 10;
					break;
				case 3:
					monsterId = 11;
					break;
				case 4:
					monsterId = 12;
					break;
				case 5:
					monsterId = 19;
					break;
				case 6:
					monsterId = 12;
				default:
					break;
				}
				monsterItem->id = vectorId++;
				monsterItem->monster_id = monsterId;
				monsterItem->row_index = monsterRowIdx;
				monsterItem->wave_id = i;
				m_vWaveMonsterForInfiniti.push_back(monsterItem);
			}
		}
	}
	return m_vWaveMonsterForInfiniti;
}

// 计算无限模式的副本波数表 
std::vector<ConfigCopyWaveItem*> DataPool::createCopyWaveForInfiniti(u32 copyId, float time)
{
	// std::vector<const ConfigCopyWaveItem*>  m_vCopyWaveForInfiniti;
	if (m_vCopyWaveForInfiniti.size()>0)
	{
		m_vCopyWaveForInfiniti.clear();
	}
	
	if (copyId == 25) // 无限模式是第25关
	{
		// m_nCount; // 用来标记每多少波把时间调整一次
		int vectorId = 0;  // 对应copy_wave_index
		float CurWareTime = time;
		for (u32 i = 0; i < 10; ++i)
		{	
			ConfigCopyWaveItem* copyWaveItem = new ConfigCopyWaveItem();
			++m_nCount;
			if (m_nCount >= 10)  // 每10波修改一次难度
			{
				if (m_timeWithMonsterHp >= 0.2f)
				{
					m_timeWithMonsterHp -= 0.1f;
					m_nCount = 0;
				}
			}
			copyWaveItem->copy_id = copyId;
			copyWaveItem->copy_wave_index = vectorId++;
			copyWaveItem->copy_wave_launch_time = CurWareTime;
			copyWaveItem->id = i;
			m_vCopyWaveForInfiniti.push_back(copyWaveItem);

			std::vector<ConfigWaveMonsterItem*>::iterator iter = m_vWaveMonsterForInfiniti.begin();
			for (; iter != m_vWaveMonsterForInfiniti.end(); ++iter)
			{
				if ((*iter)->wave_id == i)  // 如果波数id相同
				{
					switch ((*iter)->monster_id)
					{
					case 2:
						CurWareTime += (2*m_timeWithMonsterHp);
						break;
					case 10:
						CurWareTime += (2*m_timeWithMonsterHp);
						break;
					case 11:
						CurWareTime += (3*m_timeWithMonsterHp);
						break;
					case 12:
						CurWareTime += (4*m_timeWithMonsterHp);
						break;
					case 19:
						CurWareTime += (5*m_timeWithMonsterHp);
						break;
					case 18: 
						CurWareTime += (100*m_timeWithMonsterHp);
						break;
					case 21: 
						CurWareTime += (140*m_timeWithMonsterHp);
						break;
					case 22: 
						CurWareTime += (140*m_timeWithMonsterHp);
						break;
					case 23: 
						CurWareTime += (140*m_timeWithMonsterHp);
						break;
					default:
						break;
					}
				}
			}
		}
	}	
	return m_vCopyWaveForInfiniti;
}

std::vector<ConfigWaveMonsterItem*> DataPool::getCurWaveMonsterItem(u32 waveId)
{
	std::vector<ConfigWaveMonsterItem*> vCurWaveMonster;
	std::vector<ConfigWaveMonsterItem*>::iterator iter = m_vWaveMonsterForInfiniti.begin();
	for (; iter != m_vWaveMonsterForInfiniti.end(); ++iter)
	{
		if ((*iter)->wave_id == waveId)
		{
			vCurWaveMonster.push_back(*iter);
		}
	}
	return vCurWaveMonster;

}