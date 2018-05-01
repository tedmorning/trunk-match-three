#include "MonsterLayer.h"
#include "DataPool.h"
#include <algorithm>
#include "SoundManager.h"
#include "CallbackData.h"

#define MOSNTER_ATK_SHIELD_EFFECT "monster_atk_shield.mp3"
const float kMonsterSchdulerInterval = 0.2f;
const float kDefaultMonsterSize = 120.f;

const float kDefaultLayerWidth = 310.f;
const int kIceSpriteTag = 0x0010;
const int kDefenceSpriteTag = 0x0011;

const int kBossSkillActionTag = 0x0012;
const int kShieldBeAttackedActionTag = 0x0020;
const int kMonsterAttackObjectOfShield = 1;

const int kInfinitiTag = 7;
// �Ѷ�ϵ��
const float secondForMonsterHP = 1.2f;

MonsterLayer::MonsterLayer(int nCopyId)
: m_pDelegate(NULL)
, m_nCopyId(nCopyId)
, m_nCurWave(1)
, m_fAccTime(0.f)
, m_bMonsterPause(false)
, m_pNextMonsterDataManager(NULL)
, m_bUpdateMonsterWave(false)
, m_MaxWare(0)
, m_skillLaunchTime(0.f)
, m_MonsterSkill(NULL)
, m_IsBossSkillCanSend(true)
, m_IsBossSkillRuning(false)
, m_BigDogCount(0) 
, m_nShowCurWave(0)
{
	SoundManager::shareSoundManager().preloadEffect(MOSNTER_ATK_SHIELD_EFFECT);
}

MonsterLayer::~MonsterLayer()
{
}

MonsterLayer* MonsterLayer::create(int nCopyId)
{
	MonsterLayer* layer = new MonsterLayer(nCopyId);
	if (layer->init())
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

MonsterNode* MonsterLayer::getMonsterNode(int id)
{
	for (unsigned int i = 0; i < getChildrenCount(); ++i)
	{
		MonsterNode* pMonster = dynamic_cast<MonsterNode*>(this->getChildren()->objectAtIndex(i));
		if (pMonster && pMonster->getMonsterData()->getId() == id)
		{
			return pMonster;
		}
	}
	return NULL;
}

MonsterNode* MonsterLayer::getBeAttackedMonster(int aggress, const std::vector<int>& vFilterMonster)
{
	MonsterData* pPrevMonsterData = NULL;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	CCLOG("m_mMonster.count = %d", m_mMonster.size());
	for (; iter != m_mMonster.end(); ++iter)
	{
		MonsterDataManager* pMonsterManager = iter->second;
		if (!pMonsterManager->isComming() || pMonsterManager->isOver())
		{
			CCLOG("continue");
			continue;
		}

		const std::vector<MonsterData*>& refMonsterData = pMonsterManager->getMonsterDataList();
		std::vector<MonsterData*>::const_iterator monster_iter = refMonsterData.begin();
		CCLOG("refMonsterData.count = %d", refMonsterData.size());
		for (; monster_iter != refMonsterData.end(); ++monster_iter)
		{
			MonsterData* pMonsterData = *monster_iter;
			bool a = pMonsterData->isAttackEnabled();
			bool b = pMonsterData->isDead();
			bool c = pMonsterData->isOwnUser();
			if (pMonsterData->isAttackEnabled() && !pMonsterData->isDead() && !pMonsterData->isOwnUser())
			{
				std::vector<int>::const_iterator filter_iter = std::find(vFilterMonster.begin(), vFilterMonster.end(), pMonsterData->getId());
				if (filter_iter != vFilterMonster.end())
				{
					pPrevMonsterData = pMonsterData;
					continue;
				}

				MonsterNode* pMonsterNode = getMonsterNode(pMonsterData->getId());
				return pMonsterNode;
			}else if(!pMonsterData->isDead() && !pMonsterData->isOwnUser()){

			}
		}
	}

	if (pPrevMonsterData)
	{
		MonsterNode* pMonsterNode = getMonsterNode(pPrevMonsterData->getId());
		return pMonsterNode;
	}

	return NULL;
}

int MonsterLayer::getAppearAliveMonsterNum()
{
	int alive_num = 0;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	for (; iter != m_mMonster.end(); ++iter)
	{
		if (!iter->second->isComming() || iter->second->isOver())
		{
			continue;
		}
		const std::vector<MonsterData*>& refMonsterData = iter->second->getMonsterDataList();
		std::vector<MonsterData*>::const_iterator monster_iter = refMonsterData.begin();
		for (; monster_iter != refMonsterData.end(); ++monster_iter)
		{
			if (!(*monster_iter)->isDead() && !(*monster_iter)->isOwnUser())
			{
				++alive_num;
			}
		}
	}
	return alive_num;
}

bool MonsterLayer::isAppearOver()
{
	if (m_pNextMonsterDataManager && !m_pNextMonsterDataManager->isComming())
	{
		return false;
	}

	for (unsigned int i = 0; i < this->getChildrenCount(); ++i)
	{
		MonsterNode* pMonsterNode = dynamic_cast<MonsterNode*>(this->getChildren()->objectAtIndex(i));
		if (pMonsterNode)
		{
			if (!pMonsterNode->getMonsterData()->isDead() && !pMonsterNode->getMonsterData()->isOwnUser())
			{
				return false;
			}
		}
	}
	return true;
}

bool MonsterLayer::isExsitNextWave()
{
	MonsterDataManager* pNextMonsterWaveData = getNextWaveMonsterData();
	if (pNextMonsterWaveData)
	{
		if (m_fAccTime > pNextMonsterWaveData->getAppearTime())
		{
			return false;
		}
		return true;
	}
	return false;
}

void MonsterLayer::nextWave()
{
	if (m_pNextMonsterDataManager)
	{
		m_nCurWave++;
		
		createNewWaveMonster(m_pNextMonsterDataManager);
		m_bMonsterPause = false;
	}
}

MonsterDataManager* MonsterLayer::getNextWave()
{
	MonsterDataManager* pNextMonsterWaveData = getNextWaveMonsterData();
	if (pNextMonsterWaveData)
	{
		if (m_fAccTime > pNextMonsterWaveData->getAppearTime())
		{
			return NULL;
		}

		m_fAccTime = pNextMonsterWaveData->getAppearTime();

		return pNextMonsterWaveData;
	}
	return NULL;
}

MonsterDataManager* MonsterLayer::getNextWaveMonsterData()
{
	MonsterDataManager* pNextWaveData = NULL;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	for (; iter != m_mMonster.end(); ++iter)
	{
		if (iter->second->getAppearTime() > m_fAccTime)
		{
			if (pNextWaveData == NULL)
			{
				pNextWaveData = iter->second;
			}
			else
			{
				if (pNextWaveData->getAppearTime() > iter->second->getAppearTime())
				{
					pNextWaveData = iter->second;
				}
			}
		}
	}
	return pNextWaveData;
}

MonsterNode* MonsterLayer::getSkillDizzyMonster()
{
	MonsterData* pMonsterData = NULL;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	for (; iter != m_mMonster.end(); ++iter)
	{
		if (!iter->second->isComming() || iter->second->isOver())
		{
			continue;
		}

		const std::vector<MonsterData*>& refMonsterData = iter->second->getMonsterDataList();
		std::vector<MonsterData*>::const_iterator monster_iter = refMonsterData.begin();
		for (; monster_iter != refMonsterData.end(); ++monster_iter)
		{
			if (!(*monster_iter)->isDead() && !(*monster_iter)->isOwnUser() && !(*monster_iter)->isDizzy())
			{
				if (pMonsterData == NULL)
				{
					pMonsterData = *monster_iter;
				}
				else
				{
					if (pMonsterData->getBeSkillAttackedPriority() < (*monster_iter)->getBeSkillAttackedPriority())
					{
						pMonsterData = *monster_iter;
					}
				}
			}
		}
	}

	if (pMonsterData)
	{
		return getMonsterNode(pMonsterData->getId());
	}

	return NULL;
}

MonsterNode* MonsterLayer::getSkillAttackMonster()
{
	MonsterData* pMonsterData = NULL;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	for (; iter != m_mMonster.end(); ++iter)
	{
		if (!iter->second->isComming() || iter->second->isOver())
		{
			continue;
		}

		const std::vector<MonsterData*>& refMonsterData = iter->second->getMonsterDataList();
		std::vector<MonsterData*>::const_iterator monster_iter = refMonsterData.begin();
		for (; monster_iter != refMonsterData.end(); ++monster_iter)
		{
			if (!(*monster_iter)->isDead() && !(*monster_iter)->isOwnUser())
			{
				if (pMonsterData == NULL)
				{
					pMonsterData = *monster_iter;
				}
				else
				{
					if (pMonsterData->getBeSkillAttackedPriority() < (*monster_iter)->getBeSkillAttackedPriority())
					{
						pMonsterData = *monster_iter;
					}
				}
			}
		}
	}
	
	if (pMonsterData)
	{
		return getMonsterNode(pMonsterData->getId());
	}

	return NULL;
}

std::vector<MonsterNode*> MonsterLayer::getSkillGroupAttackMonster()
{
	std::vector<MonsterNode*> vBeAttackedMonster;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.begin();
	for (; iter != m_mMonster.end(); ++iter)
	{
		if (!iter->second->isComming() || iter->second->isOver())
		{
			continue;
		}

		const std::vector<MonsterData*>& refMonsterData = iter->second->getMonsterDataList();
		std::vector<MonsterData*>::const_iterator monster_iter = refMonsterData.begin();
		for (; monster_iter != refMonsterData.end(); ++monster_iter)
		{
			if (!(*monster_iter)->isDead() && !(*monster_iter)->isOwnUser())
			{
				MonsterNode* pMonster = getMonsterNode((*monster_iter)->getId());
				CCAssert(pMonster, "");
				if (pMonster)
				{
					vBeAttackedMonster.push_back(pMonster);
				}
			}
		}
	}
	return vBeAttackedMonster;
}

void MonsterLayer::addSceneStatus(SceneStatusData* data)
{
	if (data)
	{
		if (!existSceneStatus(data->getStatusType()))
		{
			m_vSceneStatus.push_back(data);
		}
	}
}

bool MonsterLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("m_ss5000_skill.plist");
	loadData(); // ��������
	return true;
}

// ���¼��㲨��������ʱ�� 
float MonsterLayer::resetMonsterWaveTime(float oldTime, std::vector<ConfigWaveMonsterItem*> curWaveMonster)
{
	std::vector<ConfigWaveMonsterItem*>::iterator it = curWaveMonster.begin();
	for (; it != curWaveMonster.end(); ++it)
	{// u32 reset_copy_wave_launch_time = 0; // secondForMonsterHP = 1.2
		switch ((*it)->monster_id)
		{
		case 2:
			oldTime += (2 *secondForMonsterHP);
			break;
		case 10:
			oldTime += (2 * secondForMonsterHP);
			break;
		case 11:
			oldTime += (3 * secondForMonsterHP);
			break;
		case 12:
			oldTime += (4 * secondForMonsterHP);
			break;
		case 18:
			oldTime += (100 * secondForMonsterHP);
			break;
		case 19:
			oldTime += (5 * secondForMonsterHP);
			break;
		case 21:
			oldTime += (120 * secondForMonsterHP);
			break;
		case 22:
			oldTime += (120 * secondForMonsterHP);
			break;
		case 23:
			oldTime += (120 * secondForMonsterHP);
			break;
		default:
			break;
		}
	}
	return oldTime;
}

void MonsterLayer::loadData()
{
	if (m_nCurWave>5 && (m_nCopyId == 25))
	{
		// m_nCurWave--;
		m_nShowCurWave += --m_nCurWave;
		m_nCurWave = 1;
	}
	
	m_mMonster.clear();

	int monster_index = 1;
	std::vector<ConfigCopyWaveItem*> vCopyWave;
	std::vector<ConfigWaveMonsterItem*> waveMonsterItem;
	float reset_copy_wave_launch_time = 2.f; // secondForMonsterHP = 1.2
	////////////////////////
	if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite) // ���������ģʽ 
	{
		waveMonsterItem = getcreateWaveMonsterForInfiniti(25);
		vCopyWave = getcreateCopyWaveForInfiniti(25, m_fAccTime);
	}
	// ��ȡ���ؿ������в���, ����ģʽ�Ļ�����copyid��������ݱ��浽�����vCopyWave��
	else
	{
		vCopyWave = getConfigCopyWave().getConfigCopyWaveItemList(m_nCopyId);
	}

	m_MaxWare = vCopyWave.size();
	CCAssert(vCopyWave.size() != 0, "��̬����δ���øø����Ĳ���");
	std::vector<ConfigCopyWaveItem*>::iterator iter = vCopyWave.begin();
	for (; iter != vCopyWave.end(); ++iter) // �������ؿ������в��� 
	{
		if (!getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite)
		{
			(*iter)->copy_wave_launch_time = reset_copy_wave_launch_time;
		}
		
		MonsterDataManager* manager = new MonsterDataManager();
		manager->setAppearTime((*iter)->copy_wave_launch_time);  // ���øò��Ĵ���ʱ�� 

		// ���ݸ����������ȡ��������� 
		std::vector<ConfigWaveMonsterItem*> refCurWaveMonster;
		if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite)
		{
			refCurWaveMonster = getCurWaveMonsterItemWithInfiniti((*iter)->id); 
		}
		else
		{
			refCurWaveMonster = getConfigWaveMonster().getWaveMonsterItemList((*iter)->id);	
			// ��Ҫ���¼���ÿһ�����������ʱ�䣬֮ǰ1.5��̫�� 
			reset_copy_wave_launch_time = resetMonsterWaveTime(reset_copy_wave_launch_time, refCurWaveMonster);
			CCLog("");
		}

		// CCAssert(refCurWaveMonster.size() != 0, "��̬��δ���øò��Ĺ�����");
		std::vector<ConfigWaveMonsterItem*>::const_iterator monster_iter = refCurWaveMonster.begin();
		for (; monster_iter != refCurWaveMonster.end(); ++monster_iter) 
		{
			const ConfigMonsterItem* pCfgMonsterItem = getConfigMonster().getConfigMonsterItem((*monster_iter)->monster_id);
			CCAssert(pCfgMonsterItem, "��̬���в����ڸù������");
			if (pCfgMonsterItem)
			{
				MonsterData* data = new MonsterData(pCfgMonsterItem);
				data->setRowIndex((*monster_iter)->row_index);
				data->setId(monster_index);
				++monster_index;

				manager->addMonsterData(data);
			}
		}

		m_mMonster[(*iter)->copy_wave_index] = manager;
	}

	schedule(schedule_selector(MonsterLayer::update), kMonsterSchdulerInterval);
	m_bUpdateMonsterWave = true;
}

void MonsterLayer::update(float time)
{
	CCLayer::update(time);

	updateMonsterWave(time);
	updateSceneStatus(time);
	updateSceneStatusAttack(time);
}

void MonsterLayer::onMonsterAttack(MonsterNode* node, int aggress)
{
	if (m_pDelegate)
	{
		m_pDelegate->onMonsterAttackWall(node, aggress);
	}
}

void MonsterLayer::onMonsterAttack(MonsterNode* attackNode, MonsterNode* beAttackedNode, int aggress)
{
	if (beAttackedNode)
	{
		CCPoint point = beAttackedNode->getPosition();
		beAttackedNode->beAttacked(aggress, 920, 150, point.x, point.y, 4);
	}
}

void MonsterLayer::onMonsterAttackShield(MonsterNode* node)
{
	// TODO: ���Ƿ���ڣ�������ڣ�����һ�£�Ȼ���������
	ShieldLayer* shield = dynamic_cast<ShieldLayer*>(this->getChildByTag(kDefenceSpriteTag));
	if (shield)
	{ 
		SoundManager::shareSoundManager().playEffect(MOSNTER_ATK_SHIELD_EFFECT);
		int rowIndex2 = -1;
		if (node->getMonsterData()->isBoss()) {
			rowIndex2 = node->getRowIndex() + 1;
		}
		shield->beAttacked(node->getRowIndex(), rowIndex2);
	}
}

void MonsterLayer::onMonsterDead(MonsterNode* node) // ���ﱻ����
{
	// ���ý��
	int monsterBeginHp = node->getMonsterData()->getBeginHp(); // ��ȡ�ֵĳ�ʼѪ��
	int gold = monsterBeginHp / 20;
	// const int gold = getUserStoreData().getGold() + (monsterBeginHp / 20);
	//getUserStoreData().setGold(gold);
	// getUserStoreData().saveUserData();
	if (node->getMonsterData()->isBoss())
	{
		gold = monsterBeginHp / 50;
	}
	if (m_pDelegate)
	{
		m_pDelegate->onUserGoldChanged(gold);
	}
	
	if (node)
	{
		node->removeFromParent();
	}

	if (isAppearOver())
	{
		if (!isExsitNextWave())
		{
			// ��������
			if (m_pDelegate)
			{   
				m_pDelegate->onMonsterOver();
			}
		}
		else
		{
			m_pNextMonsterDataManager = getNextWave();
			if (m_pDelegate && m_pNextMonsterDataManager)
			{
				m_bMonsterPause = true;
				if (this->getTag() == kInfinitiTag)
				{
					m_pDelegate->onNewWave((m_nCurWave+m_nShowCurWave), m_mMonster.size());
				}
				else
				{
					m_pDelegate->onNewWave(m_nCurWave, m_mMonster.size());
				}
				
			}
			else
			{
				if (m_pNextMonsterDataManager)
				{
					nextWave();
				}
			}
		}
	}
}

void MonsterLayer::onMonsterOwnUser(MonsterNode* node)
{
	this->onMonsterDead(NULL);
}

void MonsterLayer::onMonsterLaunchSkill(MonsterNode* node, u32 monsterSkillId, CCPoint monsterPoint)
{
	m_pBoss = node;
	// m_pBoss->setTag(0x11111);
	m_MonsterSkill = getConfigMonsterSkill().getConfigMonsterSkillItem(monsterSkillId);
	schedule(schedule_selector(MonsterLayer::updateMonsterSkill), 0.02);
	srand(time(0));
	bool isBossSkillCanSend;
	if (rand()%2)
	{
		isBossSkillCanSend = true;
	}
	else
	{
		isBossSkillCanSend = false;
		m_skillLaunchTime = 0.f;
	}

	if (monsterSkillId == 1)
	{
		CCSprite* sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
		sprite->setPosition(ccp(kDefaultLayerWidth + 20, node->getPositionY()));
		
		addChild(sprite);
		CallbackData* callback_data = new CallbackData();
		callback_data->data1 = reinterpret_cast<void*>(node);

		CCActionInterval* animate = getMonsterSkillAction();
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(MonsterLayer::cbRemoveNode), NULL);
		sprite->runAction(CCSequence::create(animate, callback, NULL));
	}
	else if (monsterSkillId == 2)
	{
	

		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���״̬��

			CCActionInterval* animation = getSTGMonsterSkillAction();
			node->stopAllActions();
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);
			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbMonsterSTGLaunchSkill), reinterpret_cast<void*>(callback_data));
			CCSequence* action = CCSequence::create(animation, callback, NULL);
			m_IsBossSkillRuning = true;
			node->runAction(action);

		}
		else
		{
			CCSprite* sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
			sprite->setPosition(ccp(kDefaultLayerWidth + 20, node->getPositionY()));

			addChild(sprite);
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);

			CCActionInterval* animate = getMonsterSkillAction();
			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(MonsterLayer::cbRemoveNode), NULL);
			sprite->runAction(CCSequence::create(animate, callback, NULL));
		}
		
	}
	else if(monsterSkillId == 3) // �ٻ���Ⱥ�ļ���
	{
		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			node->stopAllActions();
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���״̬��

			const ConfigMonsterItem* pCfgMonsterItem1 = getConfigMonster().getConfigMonsterItem(m_MonsterSkill->callMonsterId);
			const ConfigMonsterItem* pCfgMonsterItem2 = getConfigMonster().getConfigMonsterItem(m_MonsterSkill->callMonsterId);
			MonsterData* data1 = new MonsterData(pCfgMonsterItem1);
			MonsterData* data2 = new MonsterData(pCfgMonsterItem2);
			data1->setSituationType(STTUATION_SPECIAL);
			data2->setSituationType(STTUATION_SPECIAL);

			MonsterDataManager* manager = new MonsterDataManager();

			if (pCfgMonsterItem1 || pCfgMonsterItem2  )
			{
				data1->setRowIndex(node->getRowIndex()+1); // ���ù�������һ�� 
				data2->setRowIndex(node->getRowIndex()-1); // ���ù�������һ�� 
				data1->setId(1);
				data2->setId(2);

				manager->addMonsterData(data1);
				manager->addMonsterData(data2);
			}
			m_mMonster[++m_MaxWare] = manager;// û��д�ⲽ�����ɴ��Ź�
			createNewWaveMonster(manager);
			manager->setComming(true);
			m_bMonsterPause = false;
			// m_bUpdateMonsterWave = true;

			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbCallMonsterSkiller), NULL);
			CCSequence* action = CCSequence::create(callback, NULL);
			node->runAction(action);
		}
	}
	else if (monsterSkillId == 4) // ���＼�ܵ���4�Ļ�   ������  
	{
		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���
			// ���о���һ��update����� һ��boolΪtrue�Ŵ������� ���б���ʱ����������Ӵ��ڣ���ɾ����
			node->stopAllActions();
			CCPoint point = node->getPosition();
			CCParticleSystem* pParticle = CCParticleSystemQuad::create("guang.plist");
			pParticle->setPosition(ccp(250, 250));
			node->addChild(pParticle, 5); // �ڹ������������ϵͳ
			CCActionInterval* move1 = CCEaseExponentialIn::create(CCMoveBy::create(0.1f, ccp(0,50)));
			CCActionInterval* move2 = CCEaseExponentialOut::create(CCMoveBy::create(0.1f, ccp(0,-50)));
			CCSequence* queue = CCSequence::create(move1, move2, NULL);
			CCRepeat* repeat = CCRepeat::create(queue, 5);
			// pParticle->runAction(action);

			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);
			CCActionInterval* move3  = CCEaseExponentialIn::create(CCMoveTo::create(0.8f, ccp(750, 50)));

			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbMonsterSkillAttack), reinterpret_cast<void*>(callback_data));
			CCSequence* attackQueue = CCSequence::create(repeat, move3, callback, NULL);
			m_IsBossSkillRuning = true;
			pParticle->runAction(attackQueue);
			
		}
	}
	else if (monsterSkillId == 5) // ����ֵļ���Ϊ5  ��ת����
	{
		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���״̬��

			node->stopAllActions();
			CCActionInterval* animation = getSSmonsterSkillAction();
			// animation->setTag(kBossSkillActionTag);
			CCRepeat* repeat = CCRepeat::create(animation, 7);
			repeat->setTag(kBossSkillActionTag);
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);
			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbMonsterLaunchRotateSkill), reinterpret_cast<void*>(callback_data));
			CCSequence* acion = CCSequence::create(repeat, callback, NULL);
			m_IsBossSkillRuning = true;
			node->runAction(acion);

		}
	}
	else if (monsterSkillId == 6)   // ������������
	{ 
		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���

			node->stopAllActions();
			CCActionInterval* animation = getSkillIdSexAction();
			animation->setTag(kBossSkillActionTag);
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);
			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbMonsterLaunchRotateSkill), reinterpret_cast<void*>(callback_data));
			CCSequence* action = CCSequence::create(animation, callback, NULL);
			action->setTag(kBossSkillActionTag);
			m_IsBossSkillRuning = true;
			node->runAction(action);
		}

	}
	else if (monsterSkillId == 7)
	{
		if (m_MonsterSkill->cool_time <= m_skillLaunchTime && isBossSkillCanSend)
		{
			m_skillLaunchTime = 0.f;
			setMonsterSituationType(node, STTUATION_SKILL); // ������Ϊ�ͷż���
			node->stopAllActions();

			CCActionInterval* animation = getSXLMonsterSkillAction();
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(node);
			CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
				MonsterLayer::cbMonsterSevenLainchSkill), reinterpret_cast<void*>(callback_data));
			CCSequence* action = CCSequence::create(animation, callback, NULL);
			m_IsBossSkillRuning = true;
			node->runAction(action);
			
		}
	}
}

void MonsterLayer::cbMonsterSevenLainchSkill(CCNode* node, void* data) // boss�ļ���id����7�Ļص�
{
	m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ��

	if (m_IsBossSkillCanSend)
	{
		CallbackData* callback_data = new CallbackData();
		callback_data->data1 = reinterpret_cast<void*>(pMonsterNode);

		for (int i = 0; i <4; ++i)
		{
			CCSprite* dog = CCSprite::create("dogAnimate1.png");
			// dog->setAnchorPoint(ccp(0.4, 0));
			dog->setAnchorPoint(ccp(0, 0));
			if (i%2)
			{
				dog->setScale(1.2);
				dog->setPosition(ccp(-150, 120*i));
			}
			else
			{
				dog->setPosition(ccp(-280, 120*i));
				dog->setScale(1.4f);
			}
			this->addChild(dog);

			CCActionInterval* Animate = getDogRunAndAtkAction(0.2f);
			CallbackData* callback_data = new CallbackData();
			callback_data->data1 = reinterpret_cast<void*>(pMonsterNode);
			CCCallFuncND* dogcallback = CCCallFuncND::create(dog, 
				callfuncND_selector(MonsterLayer::cbRemoveDogFirst), callback_data);
			CCSequence* dogQueue = CCSequence::create(Animate, dogcallback, NULL);
			dog->runAction(dogQueue);
			
		}

		CCSprite* dog4 = CCSprite::create("dogAnimate1.png");
		dog4->setAnchorPoint(ccp(0.15, 0));
		dog4->setPosition(ccp(0, 250));
		this->addChild(dog4);
		CCActionInterval* Animate4 = getDogRunAndAtkAction(0.15f);
		CCCallFuncND* dogcallback4 = CCCallFuncND::create(dog4, 
			callfuncND_selector(MonsterLayer::cbRemoveDogFirst), reinterpret_cast<void*>(callback_data));
		CCSequence* dogQueue4 = CCSequence::create(Animate4, dogcallback4, NULL);
		dog4->runAction(dogQueue4);

		CCSprite* dog3 = CCSprite::create("dogAnimate1.png");
		dog3->setAnchorPoint(ccp(0.15, 0));
		dog3->setPosition(ccp(0, 360));;
		this->addChild(dog3);
		CCActionInterval* Animate3 = getDogRunAndAtkAction(0.18f);
		CCCallFuncND* dogcallback3 = CCCallFuncND::create(dog3, 
			callfuncND_selector(MonsterLayer::cbRemoveDogFirst),reinterpret_cast<void*>(callback_data));
		CCSequence* dogQueue3 = CCSequence::create(Animate3, dogcallback3, NULL);
		dog3->runAction(dogQueue3);


		CCSprite* dog1 = CCSprite::create("dogAnimate1.png");
		dog1->setAnchorPoint(ccp(0.15, 0));
		dog1->setPosition(ccp(0, 500));
		this->addChild(dog1);
		CCActionInterval* Animate1 = getDogRunAndAtkAction(0.1f);
		CCCallFuncND* dogcallback1 = CCCallFuncND::create(dog1, 
			callfuncND_selector(MonsterLayer::cbRemoveDogFirst), reinterpret_cast<void*>(callback_data));
		CCSequence* dogQueue1 = CCSequence::create(Animate1, dogcallback1, NULL);
		dog1->runAction(dogQueue1);


		CCSprite* dog2 = CCSprite::create("dogAnimate1.png");
		dog2->setAnchorPoint(ccp(0.15, 0));
		dog1->setPosition(ccp(0, 120));
		this->addChild(dog2);
		CCActionInterval* Animate2 = getDogRunAndAtkAction(0.16f);
// 		CallbackData* callback_data = new CallbackData();
// 		callback_data->data1 = reinterpret_cast<void*>(pMonsterNode);
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
			MonsterLayer::cbDogAnimate), reinterpret_cast<void*>(callback_data));
		CCSequence* dogQueue2 = CCSequence::create(Animate2, callback, NULL);
		
		dog2->runAction(dogQueue2);
	}
	m_IsBossSkillRuning = false;
	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type); // ���û�ԭ����״̬
}

void MonsterLayer::cbMonsterSTGLaunchSkill(CCNode* node, void* data)
{
	m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ��
	CCPoint monsterPoint = pMonsterNode->getPosition();

	if (m_IsBossSkillCanSend)
	{
		///////////////////////////////////////////////////////////////////
		u32 monsterSkillSubHp = m_MonsterSkill->aggression;
		CCNode* shield = node->getParent()->getChildByTag(kDefenceSpriteTag); 
		if (shield) // ����ܴ���
		{
			onMonsterAttackShield(pMonsterNode);
		}
		else
		{
			if (m_pDelegate)
			{
				if (monsterSkillSubHp >= 500)
				{
					m_pDelegate->onMonsterLaunchSkillLetBoardShake(); // �𶯺���
				}
				m_pDelegate->onMonsterAttackWall(pMonsterNode, monsterSkillSubHp);
			}
		}

		/////////////////////////////////////////////////////////////////

		CCSprite* sprite = CCSprite::createWithSpriteFrameName("fire0001.png");
		// sprite->setPosition(ccp(kDefaultLayerWidth + 20, node->getPositionY()));
		sprite->setPosition(ccp(monsterPoint.x + 60, monsterPoint.y + 120));
		sprite->setScale(4);
		addChild(sprite);
// 		CallbackData* callback_data = new CallbackData();
// 		callback_data->data1 = reinterpret_cast<void*>(node);
		CCActionInterval* animate = getMonsterSkillAction();
		CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(MonsterLayer::cbSkillSecond), data);
		sprite->runAction(CCSequence::create(animate, callback, NULL));
	}

	m_IsBossSkillRuning = false;
	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type); // ���û�ԭ����״̬
}

void MonsterLayer::cbRemoveDogFirst(CCNode* node, void* data) // node�ǹ��������
{

	if (node) // ɾ����
	{
		node->removeFromParent();
	}
}
void MonsterLayer::cbDogAnimate(CCNode* node, void* data) 
{
	if (node)
	{
		node->removeFromParent();
	}
	if (m_IsBossSkillCanSend)
	{
		m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ
	}
	
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ��
	if (m_IsBossSkillCanSend) // ���û�б�����ס����ִ����Щ
	{
		u32 monsterSkillSubHp = m_MonsterSkill->aggression;
		CCNode* shield = pMonsterNode->getParent()->getChildByTag(kDefenceSpriteTag); 
		if (shield) // ����ܴ���
		{
			onMonsterAttackShield(pMonsterNode);
		}
		else
		{
			if (m_pDelegate)
			{
				if (monsterSkillSubHp >= 500)
				{
					m_pDelegate->onMonsterLaunchSkillLetBoardShake(); // �𶯺���
				}
				m_pDelegate->onMonsterAttackWall(pMonsterNode, monsterSkillSubHp);
			}
		}
	}

}

void MonsterLayer::cbCallMonsterSkiller(CCNode* node, void* data) // �ٻ�����ļ��ܻص� skill3
{
	MonsterNode* pMonster = reinterpret_cast<MonsterNode*>(node);
	pMonster->getMonsterDataWithoutConst()->setSituationType(m_type);
	m_skillLaunchTime = 0.f;
}

void MonsterLayer::cbMonsterLaunchRotateSkill(CCNode* node, void* data) // skill 5 6
{
	m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ��

	if (m_IsBossSkillCanSend) // ���û�б�����ס����ִ����Щ
	{
		u32 monsterSkillSubHp = m_MonsterSkill->aggression;
		CCNode* shield = node->getParent()->getChildByTag(kDefenceSpriteTag); 
		if (shield) // ����ܴ���
		{
			onMonsterAttackShield(pMonsterNode);
		}
		else
		{
			if (m_pDelegate)
			{
				if (monsterSkillSubHp >= 500)
				{
					m_pDelegate->onMonsterLaunchSkillLetBoardShake(); // �𶯺���
				}
				m_pDelegate->onMonsterAttackWall(pMonsterNode, monsterSkillSubHp);
			}
		}
	}

	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type); // ���û�ԭ����״̬
	m_IsBossSkillCanSend = true;
	m_IsBossSkillRuning = false;

}

void MonsterLayer::cbMonsterSkillAttack(CCNode* node, void* data) // ���＼�ܵ���4��ʱ��Ļص����� 
{
	// m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ 
	node->removeFromParent(); // ɾ������ϵͳ 
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ�� 
	if (m_IsBossSkillCanSend) // ���û�б�����ס��ִ����Щ����
	{
		m_skillLaunchTime = 0.f; // �ͷ���ʱ���ʱ���¿�ʼ 
		u32 monsterSkillSubHp = m_MonsterSkill->aggression;
		if (m_pDelegate)
		{
			if (monsterSkillSubHp >= 500)
			{
				m_pDelegate->onMonsterLaunchSkillLetBoardShake();
			}
			m_pDelegate->onMonsterAttackWall(pMonsterNode, 50);
		}
	}

	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type);
	// m_IsBossSkillCanSend = true;
	m_IsBossSkillRuning = false;
}

void MonsterLayer::updateMonsterSkill(float time) // ��ʱ�����ڼ���boss�Ƿ��ͷż��� 
{
	m_skillLaunchTime += time;

	if (m_MonsterSkill->id == 1) //���ܵ���1�Ĳ���boss��Ѫ����
	{
		return;
	}

	if (!m_IsBossSkillCanSend )  // ����������б�
	{
		m_pBoss->setTag(0x55555);
	}
	if (m_pBoss == NULL)
	{
		return;
	}

	if (m_pBoss->getTag() == 0x55555 && m_IsBossSkillCanSend)
	{
		m_pBoss->stopAllActions();
		/////////////////////////////////////////////////////////////////////////////////
		//�ù��﹥��
// 		CallbackData* callback_data = new CallbackData();
// 		if (m_pBoss->getParent()->getChildByTag(kDefenceSpriteTag))
// 		{
// 			callback_data = new CallbackData();
// 			callback_data->data1 = reinterpret_cast<void*>(kMonsterAttackObjectOfShield);
// 		}
// 		m_pBoss->attackAction(CCCallFuncND::create(m_pBoss, callfuncND_selector(
//			MonsterNode::cbAttackActionEnd), reinterpret_cast<void*>(callback_data)));
		//////////////////////////////////////////////////////////////////////////////////////
		m_pBoss->moveAction(CCCallFuncND::create(m_pBoss, callfuncND_selector(MonsterNode::cbMoveActionEnd), NULL));
		m_pBoss->setTag(0x66666);
		m_skillLaunchTime = 0.f;
	}
}

void MonsterLayer::onBossBeAttack() // ��boss������ʱ�� �����ͷż��ʱ�䣬���¿�ʼ��ʱ 
{
	// m_skillLaunchTime = 0.f;
}


void MonsterLayer::onBossDead()
{
	unschedule(schedule_selector(MonsterLayer::updateMonsterSkill));
}
void MonsterLayer::setMonsterSituationType(MonsterNode* node, SituationType type)
{
	MonsterData* monsterData  = node->getMonsterDataWithoutConst();
	if (monsterData->getSituationType() != STTUATION_SKILL)
	{
		m_type = monsterData->getSituationType();
	}

	monsterData->setSituationType(type);
}

CCActionInterval* MonsterLayer::getDogRunAndAtkAction(float  speed)
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("dagAnimate.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("dogAnimate1.png");
	vFrames.push_back("dogAnimate2.png");
	vFrames.push_back("dogAnimate3.png");
	vFrames.push_back("dogAnimate4.png");
	vFrames.push_back("dogAnimate5.png");
	vFrames.push_back("dogAnimate6.png");
	vFrames.push_back("dogAnimate7.png");
	vFrames.push_back("dogAnimate8.png");
	vFrames.push_back("dogAnimate9.png");
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, speed));
	return NULL;
}

CCActionInterval* MonsterLayer::getSXLMonsterSkillAction() // ˺���Ǽ��ܶ���
{

	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SXL_skill.plist");
	
	std::vector<std::string> vFrames;
	
	for (int i = 1; i < 19; i++)
	{
		const char* str = CCString::createWithFormat("m_SXl_animate%d.png",i)->getCString();
		vFrames.push_back(str);
	}
	CCAssert(vFrames.size() != 0, "");
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.17f));

}

CCActionInterval* MonsterLayer::getSTGMonsterSkillAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("STG_Skill.plist");

	std::vector<std::string> vFrames;

	for (int i = 1; i < 20; i++)
	{
		const char* str = CCString::createWithFormat("stg_animate%d.png",i)->getCString();
		vFrames.push_back(str);
	}
	CCAssert(vFrames.size() != 0, "");
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.15f));
}

CCActionInterval* MonsterLayer::getSSmonsterSkillAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("m_ss5000_skill.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("m_ss_rotate(1).png");
	vFrames.push_back("m_ss_rotate(2).png");
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.15f));

}

CCActionInterval* MonsterLayer::getSkillIdSexAction()
{
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("m_pj_skill.plist");

	std::vector<std::string> vFrames;
	vFrames.push_back("m_pj_skill(1).png");
	vFrames.push_back("m_pj_skill(2).png");
	vFrames.push_back("m_pj_skill(3).png");
	vFrames.push_back("m_pj_skill(4).png");
	vFrames.push_back("m_pj_skill(1).png");
	vFrames.push_back("m_pj_skill(2).png");
	vFrames.push_back("m_pj_skill(3).png");
	vFrames.push_back("m_pj_skill(4).png");
	vFrames.push_back("m_pj_skill(1).png");
	vFrames.push_back("m_pj_skill(2).png");
	vFrames.push_back("m_pj_skill(3).png");
	vFrames.push_back("m_pj_skill(4).png");
	vFrames.push_back("m_pj_skill(1).png");
	vFrames.push_back("m_pj_skill(2).png");
	vFrames.push_back("m_pj_skill(3).png");
	vFrames.push_back("m_pj_skill(4).png");
	vFrames.push_back("m_pj_skill(1).png");
	vFrames.push_back("m_pj_skill(2).png");
	vFrames.push_back("m_pj_skill(3).png");
	vFrames.push_back("m_pj_skill(4).png");
	vFrames.push_back("m_pj_skill(5).png");
	CCAssert(vFrames.size() != 0, "");

	CCArray* frames = new CCArray();
	frames->autorelease();
	for (unsigned int i = 0; i < vFrames.size(); ++i)
	{
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(vFrames[i].c_str());
		CCAnimationFrame* animation_frame = new CCAnimationFrame();
		animation_frame->initWithSpriteFrame(frame, 0.5f, NULL);
		frames->addObject(animation_frame);
	}
	return CCAnimate::create(CCAnimation::create(frames, 0.25f));

}

void MonsterLayer::onMonsterLaunchSkill(MonsterNode* attackMonster, MonsterNode* beAttackedNode)
{
	// �������ﶯ��
}

void MonsterLayer::onShieldAppear(ShieldLayer* shield)
{
#if 0
	for (unsigned int i = 0; i < getChildrenCount(); ++i)
	{
		MonsterNode* node = dynamic_cast<MonsterNode*>(getChildren()->objectAtIndex(i));
		if (node)
		{
			if (shield->boundingBox().intersectsRect(node->boundingBox()))
			{
				node->setPositionX(shield->boundingBox().getMaxX() + (node->getContentSize().width / 2));
			}
		}
	}
#endif
}

void MonsterLayer::onShieldDisappear(ShieldLayer* shield)
{
#if 0
	for (unsigned int i = 0; i < getChildrenCount(); ++i)
	{
		MonsterNode* node = dynamic_cast<MonsterNode*>(getChildren()->objectAtIndex(i));
		if (node)
		{
		}
	}
#endif
}

void MonsterLayer::onShieldAttack(int aggress)
{

}

void MonsterLayer::createNewWaveMonster(MonsterDataManager* data)// ������һ���Ĺ�
{
	std::map<int, std::vector<CCNode*> > mCurWaveMonster;
	const std::vector<MonsterData*>& refMonsterData = data->getMonsterDataList();
	std::vector<MonsterData*>::const_iterator iter = refMonsterData.begin();
	for (; iter != refMonsterData.end(); ++iter)
	{
		MonsterData* pMonsterData = *iter;
		if (data == NULL)
		{
			continue;
		}

		std::vector<std::string> vFrames = pMonsterData->getMoveAnimateFrames();
		CCAssert(vFrames.size() != 0, "��������Ϊ��");
		
		MonsterNode* monster = MonsterNode::create(pMonsterData);
		monster->initWithSpriteFrameName(vFrames[0].c_str());
		monster->setAnchorPoint(ccp(1.f, 0.f));
		monster->setDelegate(this);
		monster->setFlipX(true);
		monster->setLaunchAttackPos(ccp(kDefaultLayerWidth * pMonsterData->getLaunchPosPercent()
			+ monster->getContentSize().width / 3,
			kDefaultMonsterSize * pMonsterData->getRowIndex()-10));
		monster->setPosition(ccp(50, kDefaultMonsterSize * pMonsterData->getRowIndex()-10));

		std::map<int, std::vector<CCNode*> >::iterator cur_wave_iter = mCurWaveMonster.find(pMonsterData->getRowIndex());
		if (cur_wave_iter != mCurWaveMonster.end())
		{
			monster->setPosition(ccp(-kDefaultMonsterSize * cur_wave_iter->second.size(), 
				kDefaultMonsterSize * pMonsterData->getRowIndex()));

			cur_wave_iter->second.push_back(monster);
		}
		else
		{
			std::vector<CCNode*> vNode;
			vNode.push_back(monster);
			mCurWaveMonster[pMonsterData->getRowIndex()] = vNode;
		}

		monster->runMonsterAction();
		if (pMonsterData->isBoss())
		{
			this->addChild(monster, 2);
		}
		else
		{
			this->addChild(monster);
		}
		
	}
	data->setComming(true);
}

bool MonsterLayer::statusEmpty() const
{
	return m_vSceneStatus.empty();
}

bool MonsterLayer::isExistIceStatus() const
{
	return existSceneStatus(ST_ICE);
}

bool MonsterLayer::existSceneStatus(StatusType type) const
{
	std::vector<SceneStatusData*>::const_iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); ++iter)
	{
		if ((*iter)->getStatus(type))
		{
			return true;
		}
	}
	return false;
}

void MonsterLayer::addStatusDeltaTime(float time)
{
	std::vector<SceneStatusData*>::iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() == ST_ICE)
		{
			(*iter)->addStatusDeltaTime(time);
		}
	}
}

std::vector<SceneStatusData*> MonsterLayer::getNewSceneStatus()
{
	std::vector<SceneStatusData*> vNewScene;
	std::vector<SceneStatusData*>::iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); ++iter)
	{
		if (!(*iter)->isDeal())
		{
			vNewScene.push_back(*iter);
		}
	}
	return vNewScene;
}

std::vector<SceneStatusData*> MonsterLayer::getOverSceneStatus()
{
	std::vector<SceneStatusData*> vOverScene;
	std::vector<SceneStatusData*>::iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); ++iter)
	{
		if ((*iter)->isOver())
		{
			vOverScene.push_back(*iter);
		}
	}
	return vOverScene;
}

void MonsterLayer::clearOverSceneStatus()
{
	std::vector<SceneStatusData*>::iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); )
	{
		bool bNext = true;

		if ((*iter)->isOver())
		{
			iter = m_vSceneStatus.erase(iter);
			bNext = false;
		}

		if (bNext)
		{
			++iter;
		}
	}
}

void MonsterLayer::updateMonsterWave(float time)
{
	if (m_bMonsterPause)
	{
		return;
	}

	if (!m_bUpdateMonsterWave)
	{
		return;
	}

	m_fAccTime += time;
	std::map<int, MonsterDataManager*>::iterator iter = m_mMonster.find(m_nCurWave);
	if (iter == m_mMonster.end())
	{
		if (this->getTag() != kInfinitiTag)
		{
			m_bUpdateMonsterWave = false;
		}
		return;
	}
	if (iter->second == NULL || iter->second->getAppearTime() > m_fAccTime)
	{
		return;
	}

	m_pNextMonsterDataManager = iter->second;
	if (m_pDelegate)
	{
		m_bMonsterPause = true;
		if (this->getTag() == kInfinitiTag)
		{
				m_pDelegate->onNewWave((m_nCurWave  +m_nShowCurWave), m_mMonster.size());
		}
		else
		{
				m_pDelegate->onNewWave(m_nCurWave, m_mMonster.size());
		}
	
	}
	else
	{
		nextWave();
	}
}

void MonsterLayer::updateSceneStatus(float time)
{
	updateOverStatus(time);
	addStatusDeltaTime(time);
	updateNewStatus(time);
}

void MonsterLayer::updateOverStatus(float time)
{
	std::vector<SceneStatusData*> vOverStatus = getOverSceneStatus();
	std::vector<SceneStatusData*>::iterator iter = vOverStatus.begin();
	for (; iter != vOverStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() == ST_ICE)
		{
			this->removeChildByTag(kIceSpriteTag);
			// this->removeChildByTag(0x123);
		}
	}

	// TODO: ��������ֻɾ��ST_ICE
	clearOverSceneStatus();
}

void MonsterLayer::updateNewStatus(float time)
{
	std::vector<SceneStatusData*> vNewSceneStatus = getNewSceneStatus();
	std::vector<SceneStatusData*>::iterator iter = vNewSceneStatus.begin();
	for (; iter != vNewSceneStatus.end(); ++iter)
	{
		if ((*iter)->getStatusType() == ST_ICE)
		{
			CCSprite* sprite = CCSprite::create("ice.png");
			// CCSprite* sprite = CCSprite::create("");
			sprite->setAnchorPoint(CCPointZero);
			sprite->setPosition(ccp(10, 0));
			sprite->setTag(kIceSpriteTag);
			this->addChild(sprite, -1);
		}
		else if ((*iter)->getStatusType() == ST_DEFENCE)
		{
			ShieldLayer* shield = ShieldLayer::create(*iter, this);
			shield->setTag(kDefenceSpriteTag);
			this->addChild(shield, -1);
		}

		(*iter)->setDeal(true);
	}
}

void MonsterLayer::updateSceneStatusAttack(float time)
{
	std::vector<SceneStatusData*>::iterator iter = m_vSceneStatus.begin();
	for (; iter != m_vSceneStatus.end(); ++iter)
	{
		if (!(*iter)->isDeal())
		{
			continue;
		}

		if ((*iter)->getStatusType() == ST_DEFENCE)
		{
			
		}
		else if ((*iter)->getStatusType() == ST_ICE)
		{
			// �ж��Ƿ��й��ﾭ������
			if ((*iter)->isExistAttackStatus())
			{
				if (!(*iter)->enableAttack())
				{
					(*iter)->addAttackDeltaTime(time);
					continue;
				}

				StatusData* pDamageStatusData = (*iter)->getStatus(ST_DAMAGE);
				for (unsigned int i = 0; i < getChildrenCount(); ++i)
				{
					MonsterNode* pMonsterNode = dynamic_cast<MonsterNode*>(getChildren()->objectAtIndex(i));
					if (pMonsterNode && pMonsterNode->isOnIce())
					{
						pMonsterNode->beAttacked(1, 0, 0, 0, 0, 0);
					}
				}

				(*iter)->launchAttack();
			}
		}
	}
}

CCActionInterval* MonsterLayer::getMonsterSkillAction()
{
	CCArray* frames = new CCArray();
	frames->autorelease();
	for (int i = 1; i <= 13; ++i)
	{
		std::string str = "fire000%d.png";
		if (i >= 10)
		{
			str = "fire00%d.png";
		}

		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(str.c_str(), i)->getCString());
		CCAnimationFrame* animFrame = new CCAnimationFrame();
		animFrame->initWithSpriteFrame(frame, 1.f, NULL);
		frames->addObject(animFrame);
	}

	return CCAnimate::create(CCAnimation::create(frames, 0.05f));
}

void MonsterLayer::cbRemoveNode(CCNode* node, void* data)
{
// 	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
// 	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ�� 
// 	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type);
	if (node) {
		node->removeFromParent();
	}
}

void MonsterLayer::cbSkillSecond(CCNode* node, void* data)
{
	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1); // ��ȡ��
	m_skillLaunchTime = 0.f;
	if (node) {
		node->removeFromParent();
	}
	pMonsterNode->getMonsterDataWithoutConst()->setSituationType(m_type); // ���û�ԭ����״̬
}
