#include "FightScene.h"
#include "LoadHelper.h"
#include "DataPool.h"
#include "CallbackData.h"
#include "EffectData.h"
#include "LoseLayer.h"
#include "SoundManager.h"
#include <algorithm>
#include "game_win_event.h"
#include "BeginnersGuideLayer.h"

#define SPECIAL_GEM_EXPLODE      4
#define NORMAL_GEM_REMOVE        1

#define EFFECT_LAYER_ZORDER      10

#define BUTTON_CLINK_MUSIC       "buttonClink.mp3"
#define BATTLE_BACKGROUND_MUSIC  "battle.mp3"
#define MOSNTER_ATK_EFFECT       "monster_atk.mp3"
#define FAIL_MUSIC               "fail.mp3"
#define SUCCESS_MUSIC            "success.wav"
#define GEM_EXPLODE              "bombexplode.wav"
#define GEM_BOMB                 "bomb.mp3"
#define GEM_TENLINE              "tenline.mp3"
#define GEM_LIGHTBALL            "lightball.mp3"
#define GET_COIN_MUSIC           "getcoin.mp3"

const int kNormalAttackDamage = 50;
const int kExplodeAttackDamage = 100;
//const float kBulletMoveTime = 0.4f;
const float kBulletAcceleratedSpeed = 3000.0f;

const int kPropHint = 1;             // �Ŵ�
const int kPropEnergy = 2;			 // ����
const int kPropRefreshBoard = 3;     // ˢ��
const int kPropMushroom = 4;         // ����Ģ��
const int kHelpSpriteTag = 5;
const int kAskViewTag = 6;

const int kInfinitiTag = 7;
const int kLianjJiTag = 8;
const int kHinitTag = 9;
const int lianjiX = -120;
const int lianjiY = 500;

const float LIANJI_TIME = 1.7f;

const float FIFTY = 50;
const float EIGHTY = 80;
const float ONE_HUNDRED_AND_TWENTY = 120;
const float ONE_HUNDRED_AND_SIXTY = 160;

USING_NS_CC;

FightScene::FightScene(int nCopyId, bool isMineral)
: m_pMonsterLayer(NULL)
, m_pSkillLayer(NULL)
, m_pEffectLayer(NULL)
, m_bShowLoseLayer(false)
, m_bShowWinLayer(false)
, m_pMenuLayer(NULL)
, m_pWallLayer(NULL)
, m_pTopLayer(NULL)
, m_pEnergyLayer(NULL)
, m_pBoardLayer(NULL)
, m_nCopyId(nCopyId)
, m_nCopyWinGold(0)
, m_beginX(0)
, m_beginY(0)
, m_endX(0)
, m_endY(0)
, m_nMonsterDieJumpNum(0)
, m_delayTime(0)
, m_isParticleCreate(true)
, m_pStoreLayer(NULL)
, m_pPropLayer(NULL)
, m_pFreeGoldLayer(NULL)
, m_isFreeGoldLayerToCreateStore(false)
, m_RecordAutoRemoveGemTime(0.f)
, m_recordUserRemoveGemTime(0.f)
, m_RecordAutoRemoveGemNum(0)
, m_recordUserRemoveGemNum(0)
, m_AutoUpdateRecord(false)
, m_userScore(0.f)
, m_userScoreMultiple(0.f)
, m_lianjiCount(0)
, m_lianjiSpaceTime(0.f)
, m_AutoIsCanVisible(false)
, m_lianjiMaxClose(false)
, m_lianjiMaxCloseTime(0.f)
, m_isMineral(isMineral)
 ,m_isNewUnlockMineral(false)
{
	SoundManager::shareSoundManager().preloadBackgroundMusic(BATTLE_BACKGROUND_MUSIC);
	
	SoundManager::shareSoundManager().preloadBackgroundMusic(SUCCESS_MUSIC);	

	SoundManager::shareSoundManager().preloadEffect(FAIL_MUSIC);
	SoundManager::shareSoundManager().preloadEffect(BUTTON_CLINK_MUSIC);
	SoundManager::shareSoundManager().preloadEffect("combo1.wav");
	SoundManager::shareSoundManager().preloadEffect("combo2.wav");
	SoundManager::shareSoundManager().preloadEffect("combo3.wav");
	SoundManager::shareSoundManager().preloadEffect("combo4.wav");
	SoundManager::shareSoundManager().preloadEffect("combo5.wav");
	SoundManager::shareSoundManager().preloadEffect("combo6.wav");
	SoundManager::shareSoundManager().preloadEffect("combo7.wav");
	SoundManager::shareSoundManager().preloadEffect("combo8.wav");
	SoundManager::shareSoundManager().preloadEffect("combo9.wav");
	SoundManager::shareSoundManager().preloadEffect("combo10.wav");

	SoundManager::shareSoundManager().preloadEffect(MOSNTER_ATK_EFFECT);
	SoundManager::shareSoundManager().preloadEffect(GEM_EXPLODE);
	SoundManager::shareSoundManager().preloadEffect(GEM_BOMB);
	SoundManager::shareSoundManager().preloadEffect(GEM_TENLINE);
	SoundManager::shareSoundManager().preloadEffect(GEM_LIGHTBALL);
		
	SoundManager::shareSoundManager().preloadEffect(GET_COIN_MUSIC);
}

FightScene::~FightScene()
{
// 	if (m_pDelegate) {
// 		m_pDelegate->onExitFightScene(m_nCopyId);
// 	}
}

CCScene* FightScene::scene(int nCopyId,  bool isMineral, FightSceneDelegate* delegate)
{
    CCScene* scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(!scene);

		FightScene *layer = FightScene::create(nCopyId,isMineral, delegate);
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

FightScene* FightScene::create(int nCopyId,bool isMineral, FightSceneDelegate* delegate)
{
	FightScene* scene = new FightScene(nCopyId, isMineral);
	if (scene && scene->init())
	{
		scene->setDelegate(delegate);
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return NULL;
}

bool FightScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	return true;
}

void FightScene::update(float delta)
{
	m_RecordAutoRemoveGemTime += delta;
	m_recordUserRemoveGemTime += delta;
	if (m_recordUserRemoveGemTime >= LIANJI_TIME)
	{
		for (int i = 101; i<106;++i)
		{
			getChildByTag(i)->setVisible(false); 
		}
		m_recordUserRemoveGemTime = 0.f; 
		m_recordUserRemoveGemNum = 0;  
		m_RecordAutoRemoveGemNum = 0; 
		m_lianjiCount = 0; 
		m_AutoIsCanVisible = false; 
	}

// 	if (m_lianjiMaxClose)
// 	{
// 		m_lianjiMaxCloseTime += delta;
// 		if (m_lianjiMaxCloseTime >= 2.f)
// 		{
// 			getChildByTag(100)->setVisible(false);
// 			getChildByTag(100)->setPosition(ccp(lianjiX, lianjiY));
// 			getChildByTag(kLianjJiTag)->setVisible(false);
// 			m_lianjiMaxClose = false;
// 			m_lianjiMaxCloseTime = 0.f;
// 		}
// 	}
	
}

void FightScene::back()
{
	CCLog("");
}

void FightScene::cbAutoRemoveGemEffect(CCNode* node, void* data)
{
	if (node)
	{
		if (m_RecordAutoRemoveGemNum == 0)
		{
		    node->setVisible(false);
		    node->setPosition(ccp(lianjiX, lianjiY));
		}
	
// 		if (m_RecordAutoRemoveGemNum > 50)
// 		{
// 			m_lianjiMaxClose = true;
// 		}
		
		int count = 0;
		for (int i = 51; i< 141; ++i)
		{
			if (!this->getChildByTag(i)->isVisible())
			{
				++count;
			}
			if (count == 90)
			{
				m_AutoIsCanVisible = false;
				this->getChildByTag(kLianjJiTag)->setVisible(false);
				// m_lianjiCount = 0; // ���������createLianJiEfffct()�����м���
			}
		}	
	}
}

void FightScene::setAutoRemoveGemVisible()
{
	if (m_RecordAutoRemoveGemNum > 90)
	{
		return;
	}
	
	for (int i = 51; i<(m_RecordAutoRemoveGemNum + 50); ++i)
	{
		this->getChildByTag(i)->setVisible(false);
		this->getChildByTag(i)->setPosition(ccp(lianjiX, lianjiY));
	}
	CCMoveTo* move1 = CCMoveTo::create(0.1f, ccp(lianjiX + 360, lianjiY));
	CCEaseExponentialOut* move2 = CCEaseExponentialOut::create(move1);
	// CCEaseExponentialIn* move2 = CCEaseExponentialIn::create(move1);
	this->getChildByTag(m_RecordAutoRemoveGemNum + 50)->setVisible(true);
	CCCallFuncND* callback = CCCallFuncND::create(this, 
		callfuncND_selector(FightScene::cbAutoRemoveGemEffect),NULL );
	CCSequence* action = CCSequence::create(move2, CCDelayTime::create(LIANJI_TIME), callback, NULL);
	this->getChildByTag(m_RecordAutoRemoveGemNum + 50)->runAction(action);

}
void FightScene::setUserRemoveGemVisible()
{
	for (int i = 101; i < 106; ++i)
	{
		this->getChildByTag(i)->setVisible(false);
	}
	for (int i = 101; i < 101 + m_recordUserRemoveGemNum; ++i)
	{
		this->getChildByTag(i)->setVisible(true);
	}
}

void FightScene::onEnter()
{
	m_userScore = 0;
	CCLayer::onEnter();
	CCLog("FightScene::onEnter");

	const ConfigCopyItem* pCfgCopyItem = getConfigCopy().getConfigCopyItem(m_nCopyId);
	if (pCfgCopyItem == NULL) {
		return;
	}

	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("monster.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gem_effect.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("wall_be_attacked.plist");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skillIcon.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fight_ani.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fightAni.plist");
	// CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("vegetables.plist");
	// CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("explode.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("normal_attack.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("props.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("effect_hypercube.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("effect_lighting.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("lianjipng.plist");
	SoundManager::shareSoundManager().playBackgroundMuisc(BATTLE_BACKGROUND_MUSIC, true);

	m_isCopyUnLockBefore = false;

	// ������ս������ͼ
	CCSprite* bg = new CCSprite();
	if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite)// ���������ģʽ 
	{
		bg = CCSprite::create("main.png");
		bg->setAnchorPoint(CCPointZero);
		bg->setPosition(CCPointZero);
		this->addChild(bg);
	}
	else
	{
		bg = CCSprite::create("main.png");
		bg->setAnchorPoint(CCPointZero);
		bg->setPosition(CCPointZero);
		this->addChild(bg);
	}

	Android* and = new Android();
	and->setDelegate(this);

	// ��������
	m_pBoardLayer = BoardLayer::create(pCfgCopyItem->gem_num, m_isMineral);
	m_pBoardLayer->setPosition(ccp(385, 30));
	m_pBoardLayer->setDelegate(this);
	m_pBoardLayer->start();
	this->addChild(m_pBoardLayer,2);

	// �������߼��������۵��Ǹ����
// 	CCSprite* left_bg = CCSprite::create("left.png");
// 	left_bg->setAnchorPoint(ccp(1, 0));
// 	left_bg->setPosition(ccp(950, 55));
// 	this->addChild(left_bg);

	// ����������
	m_pSkillLayer = SkillLayer::create(m_nCopyId);
	m_pSkillLayer->setAnchorPoint(CCPointZero);
	m_pSkillLayer->setPosition(ccp(836, 45));
	m_pSkillLayer->setDelegate(this);
	this->addChild(m_pSkillLayer, 2);

	m_pPropLayer = PropLayer::create(m_nCopyId);
	m_pPropLayer->setAnchorPoint(CCPointZero);
	m_pPropLayer->setPosition(ccp(400, 550));
	m_pPropLayer->setDelegate(this);
	this->addChild(m_pPropLayer);

	m_pEffectLayer = EffectLayer::create();
	m_pEffectLayer->setDelegate(this);
	m_pEffectLayer->setPosition(CCPointZero);
	this->addChild(m_pEffectLayer, 2);

	m_pEnergyLayer = EnergyLayer::create();
	m_pEnergyLayer->setDelegate(this);
	m_pEnergyLayer->setPosition(ccp(826, 308));
	m_pEnergyLayer->setAnchorPoint(CCPointZero);
	this->addChild(m_pEnergyLayer);

	if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite) // ���������ģʽ
	{
		m_pTopLayer = TopLayer::create(pCfgCopyItem->hp, pCfgCopyItem->wave_count, true);	
	}
	else
	{
		m_pTopLayer = TopLayer::create(pCfgCopyItem->hp, pCfgCopyItem->wave_count);
	}
	m_pTopLayer->setDelegate(this);
	m_pTopLayer->setPosition(ccp(0, 580));
	this->addChild(m_pTopLayer, 1);
	

	m_pWallLayer = WallLayer::create(pCfgCopyItem->hp);
	m_pWallLayer->setDelegate(this);
	m_pWallLayer->setPosition(ccp(315, 29));
	this->addChild(m_pWallLayer);

	m_pMonsterLayer = MonsterLayer::create(m_nCopyId);
	m_pMonsterLayer->setPosition(ccp(-10, 65));
	m_pMonsterLayer->setDelegate(this);

	this->addChild(m_pMonsterLayer, 2);
	if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite)
	{
		m_pMonsterLayer->setTag(kInfinitiTag);
	}
	
	for (int i = 101; i < 106; ++i)
	{
		CCSprite* sprite = CCSprite::create("hint.png");
		sprite->setPosition(ccp(360 + ((i-100)*90), 545));
		sprite->setTag(i);
		this->addChild(sprite);
		sprite->setVisible(false);
	}
	for (int i = 51; i < 141; ++i)
	{
		const char* str = CCString::createWithFormat("%d.png",i-50)->getCString();
		CCSprite* sprite = CCSprite::createWithSpriteFrameName(str);
		sprite->setTag(i);
		sprite->setPosition(ccp(lianjiX, lianjiY));
		sprite->setAnchorPoint(ccp(1,0.5));
		this->addChild(sprite, 2);
		sprite->setVisible(false);
		// sprite->setScale(0.6f);
	}
	CCSprite* lianji = CCSprite::createWithSpriteFrameName("lianji.png");
	lianji->setPosition(ccp(lianjiX + 400, lianjiY));
	lianji->setTag(kLianjJiTag);
	this->addChild(lianji, 2);

	lianji->setVisible(false);
	lianji->setScale(0.6f);

	schedule(schedule_selector(FightScene::update), 0.01f);
}

void FightScene::onExit()
{
	CCLayer::onExit();

	SimpleAudioEngine::sharedEngine()->end();
}

void FightScene::createLianJiEfffct() // �����ж� 
{
	m_recordUserRemoveGemTime = 0;
	if (m_pBoardLayer->isUserMoveHaveonCombo()) // �û�����������ɵ�����
	{
		if ((m_lianjiCount == 0) && (!m_AutoIsCanVisible))
		{
			// Ҫע�����ı����0 
			++m_lianjiCount; 
			m_AutoIsCanVisible = true;
			m_pBoardLayer->setUserMoveGem(false);
			return; 
		}

		//if (m_recordUserRemoveGemTime <= LIANJI_TIME)
		//{
			++m_RecordAutoRemoveGemNum;
			if (m_recordUserRemoveGemNum < 5)
			{
				//if (m_lianjiCount != 0)
				//{
					++m_recordUserRemoveGemNum;
				//}
			}
		//}
		//else
		//{
			//m_recordUserRemoveGemNum = 0;
			//m_RecordAutoRemoveGemNum = 0;
		//}
		setUserRemoveGemVisible();
		m_recordUserRemoveGemTime = 0.f;
		m_pBoardLayer->setUserMoveGem(false);
	}
	else  // �Զ�����(�����Զ����У���ը��ͬɫ����) 
	{
		m_recordUserRemoveGemTime = 0.f; // �Զ�������ʱ���û���������ͣ 
		++m_RecordAutoRemoveGemNum;
		m_RecordAutoRemoveGemTime = 0.f;	
	}
	this->getChildByTag(kLianjJiTag)->setVisible(true);
	setAutoRemoveGemVisible();
}

void FightScene::onCombo(int combo)
{
	if (m_pBoardLayer)
	{
		m_pBoardLayer->setUserRestRemoveTime(0.f);
		// createLianJiEfffct();
	}

	if (m_pPropLayer)
	{
		m_pPropLayer->setFirstPropShanDown(true);
		// m_pPropLayer->setThirePropShanDown(true);
	}

	if(combo > 10)
	{
		combo = 10;
	}
	const char* file = CCString::createWithFormat("combo%d.wav", combo)->getCString();
	SoundManager::shareSoundManager().playEffect(file);

}

// �������ӱ�ը 
// |first|��ը���ͣ�|second|��ը������ 
void FightScene::onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem, int boardSendToFightAttackNum)
{
	createLianJiEfffct();

	switch (m_recordUserRemoveGemNum)
	{
	case 0:
		m_userScoreMultiple = 0.f;
		break;
	case 1:
		m_userScoreMultiple = 0.1f;
		break;
	case 2:
		m_userScoreMultiple = 0.2f;
		break;
	case 3:
		m_userScoreMultiple = 0.3f;
		break;
	case 4:
		m_userScoreMultiple = 0.4f;
		break;
	case 5:
		m_userScoreMultiple = 0.5f;
		break;
	default:
		break;
	}

	// bug: һ�λ�����������ͨ�������������ȴֻ������һ��
	if (m_pMonsterLayer) 
	{
		if (m_pEnergyLayer) {
			int nExplodeNum = 0;
			std::map<GemCellRemoveType, std::vector<GemCellEx*> >::iterator iter = mExplodeGem.begin();
			for (; iter != mExplodeGem.end(); ++iter) {
				nExplodeNum += iter->second.size();
			}
			m_pEnergyLayer->addEnergy(nExplodeNum);// ����������
		}

		std::map<GemCellRemoveType, std::vector<GemCellEx*> >::iterator iter = mExplodeGem.begin();
		for (; iter != mExplodeGem.end(); ++iter) {
			if (iter->second.empty()) {
				continue;
			}

			static int nAttackNum = getConst("GEM_NORMAL_ATK_NUM");        // ��ȡ�������������� 
			static int nAggression = getConst("GEM_NORMAL_AGGRESSION");    // ��ȡ���ӵĻ��������� 
			static int nMonsterDieJumpNum = 0;                             // ��������ʱ�򵯵Ĵ���

			switch (iter->first)
			{
			case GCRT_NORMAL:					// ��ͨ����
				nAttackNum = getConst("GEM_NORMAL_ATK_NUM"); 
				if (boardSendToFightAttackNum > nAttackNum)
				{
					nAttackNum = boardSendToFightAttackNum;
				}
				nAggression = getConst("GEM_NORMAL_AGGRESSION");  
				nMonsterDieJumpNum = 0;

				m_userScore += (FIFTY + (FIFTY * m_userScoreMultiple));
				break;
			case GCRT_EXPLOSIVE:				// ��ը����
				nAggression = getConst("GEM_EXPLOSIVE_AGGRESSION");
				nAttackNum = getConst("GEM_EXPLOSIVE_ATK_NUM");
				nMonsterDieJumpNum = 1;

				m_userScore += (EIGHTY + (EIGHTY * m_userScoreMultiple));
				break;
			case GCRT_ROWCOLUMNREMOVE:			// ��������
				nAggression = getConst("GEM_ROW_COLUMN_REMOVE_AGGRESSION");
				nAttackNum = getConst("GEM_ROW_COLUMN_REMOVE_ATK_NUM");
				nMonsterDieJumpNum = 2;

				m_userScore += (ONE_HUNDRED_AND_TWENTY + (ONE_HUNDRED_AND_TWENTY * m_userScoreMultiple));
				break;
			case GCRT_HYPERCUBE:				// ͬɫ����
				nAggression = getConst("GEM_HYPERCUBE_AGGRESSION");
				nAttackNum = getConst("GEM_HYPERCUBE_ATK_NUM");
				nMonsterDieJumpNum = 3;

				m_userScore += (ONE_HUNDRED_AND_SIXTY + (ONE_HUNDRED_AND_SIXTY * m_userScoreMultiple));
				break;
			case GCRT_AUTO:						// �Զ�����
				nAttackNum = getConst("GEM_NORMAL_ATK_NUM"); 
				nAggression = getConst("GEM_NORMAL_AGGRESSION"); 

				m_userScore += (FIFTY + (FIFTY * m_userScoreMultiple));
				break;
			case GCRT_EXPLOSIVE_COMPOSE:		// ��ը�ϳ�
				nAttackNum = getConst("GEM_NORMAL_ATK_NUM"); 
				nAggression = getConst("GEM_EXPLOSIVE_COMPOSE");

				m_userScore += (EIGHTY + (EIGHTY * m_userScoreMultiple));
				break;
			case GCRT_ROWCOLUMNREMOVE_COMPOSE:	// ���кϳ�
				nAttackNum = getConst("GEM_NORMAL_ATK_NUM"); 
				nAggression = getConst("GEM_ROW_COLUMN_REMOVE_COMPOSE");

				m_userScore += (ONE_HUNDRED_AND_TWENTY + (ONE_HUNDRED_AND_TWENTY * m_userScoreMultiple));
				break;
			case GCRT_HYPERCUBE_COMPOSE:		// ͬɫ�ϳ�
				nAttackNum = getConst("GEM_NORMAL_ATK_NUM"); 
				nAggression = getConst("GEM_HYPERCUBE_COMPOSE");

				m_userScore += (ONE_HUNDRED_AND_SIXTY+ (ONE_HUNDRED_AND_SIXTY * m_userScoreMultiple));
				break;
			default:
				break;
			}
			 int MonsterInWindowsNum = m_pMonsterLayer->getAppearAliveMonsterNum(); // �ǻ�ȡ��Ļ���ж���ֻ���ŵĹ�
			 int minAttackNum = MIN(nAttackNum, MonsterInWindowsNum);   //  �����Ҫ�����Ĺֵĸ���
			 int count = 0;

			 while(nAttackNum && MonsterInWindowsNum) // 
			 {
				 ++count;
				 if (count >= 30)
				 {
					 CCLog("debug");
					 break;
				 }
				 
				++m_delayTime;
				minAttackNum = MIN(nAttackNum, MonsterInWindowsNum);   //  �����Ҫ�����Ĺֵĸ���
				if (minAttackNum  == 0)
				{
					break;
				}
				GemCellEx* gem = *iter->second.begin(); // ��ȡҪ��ը������?
				std::vector<int> vFilterMonster;
				for (int i = 0; i < minAttackNum; ++i) {
					// this->schedule(schedule_selector( FightScene::updateMonsterNum));
					MonsterNode* pMonsterNode = m_pMonsterLayer->getBeAttackedMonster(nAggression, vFilterMonster);
					if (pMonsterNode == NULL)
					{
						continue;
					}

					int hp = pMonsterNode->getMonsterData()->getHp();
					int n = hp - nAggression;
					if (hp - nAggression <= 0)
					{
						
						--MonsterInWindowsNum;
					}
					pMonsterNode->beforeBeAttacked(nAggression);

					vFilterMonster.push_back(pMonsterNode->getMonsterData()->getId());

					CCSize size = gem->getContentSize();
			
					// ����һ������ϵͳ 
					CCParticleSystem* pParticle = CCParticleSystemQuad::create(getParticleSystemFileName(iter->first).c_str());
					CCPoint gemSize = gem->convertToWorldSpace(ccp(size.width / 2, size.height / 2));
					pParticle->setPosition(gem->convertToWorldSpace(ccp(size.width / 2, size.height / 2)));
					this->addChild(pParticle, 10);

					// �������� 
					size = pMonsterNode->getContentSize();
					CCPoint end = pMonsterNode->convertToWorldSpace(ccp(size.width / 2, size.height / 2));
					CallbackData* callback_data = new CallbackData();
					callback_data->data1 = reinterpret_cast<void*>(pMonsterNode);
					callback_data->data2 = reinterpret_cast<void*>(nAggression);
					int beginX = gemSize.x;
					int beginY = gemSize.y;
					
					int endX = end.x;
					int endY = end.y;
					callback_data->data3 = reinterpret_cast<void*>(beginX);   // ��������ʼλ��x 
					callback_data->data4 = reinterpret_cast<void*>(beginY);       // ��������ʼλ��y 
					callback_data->data5 = reinterpret_cast<void*>(endX);   // �����Ľ�ֹλ��x  
					callback_data->data6 = reinterpret_cast<void*>(endY);       // �����Ľ�ֹλ��y  
					callback_data->data7 = reinterpret_cast<void*>(nMonsterDieJumpNum);

					m_beginX = beginX;
					m_beginY = beginY;
					m_endX = endX;
					m_endY = endY;
					m_nMonsterDieJumpNum = nMonsterDieJumpNum;

					int dx = beginX - endX;
					int dy = beginY - endY;
					float fBulletMoveDistance = sqrt(dx*dx + dy*dy);
					float fBulletMoveTime = sqrt(2*fBulletMoveDistance/kBulletAcceleratedSpeed);
					CCActionInterval* move = CCEaseExponentialIn::create(CCMoveTo::create(fBulletMoveTime, end));
					CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(
						FightScene::cbMoveBulletEnd), reinterpret_cast<void*>(callback_data));
					CCSequence* seq1 = CCSequence::create(move, callback, NULL);
					CCSequence* seq2 = CCSequence::create(CCDelayTime::create(0.5f), move, callback, NULL );

					if (m_delayTime > 1)
					{
						pParticle->runAction(seq2); // ���ι���
						--nAttackNum;
					}
					else
					{
						pParticle->runAction(seq1);
						--nAttackNum;
					}
					
				}
			}
		}
	}
	m_delayTime = 0;
}


void FightScene::onBoardImmobile()
{
	// TODO: ���̲����ƶ���
	CCLog("immobile");
	// CCDirector::sharedDirector()->pause(); // ��������ƶ��˾���ͣ����ʾ�û��Ƿ�ˢ��
	CCSprite* hint = CCSprite::create("refresh.png");
	hint->setPosition(ccp(480, 320));
	hint->setTag(kHinitTag);
	this->addChild(hint, 5);
	CCDelayTime* delay = CCDelayTime::create(3.f);
	CCCallFuncND* callback = CCCallFuncND::create(this, callfuncND_selector(FightScene::cbBoardImmobileHint), NULL);
	CCSequence* action = CCSequence::create(delay, callback, NULL);
	hint->runAction(action);

	if (m_pPropLayer)
	{
		m_pPropLayer->setRefreshPropBlink();
	}

	//	ˢ������
// 	if (m_pBoardLayer) {
// 		m_pBoardLayer->refresh();
// 	}
	
}

void FightScene::cbBoardImmobileHint(CCNode* node, void* data)
{
	// ɾ����ʾ�û�ʹ��ˢ�¾���Ķ���
	if (node)
	{
		node->removeFromParent();
	}
	
}
void FightScene::onExplosiveCombo()
{
	CCLog("explosive combo");
	this->runAction(getShakeAction());

	SoundManager::shareSoundManager().playEffect(GEM_BOMB);
}

void FightScene::onRowColumnRemoveCombo()
{
	CCLog("row column remove combo");
	this->runAction(getShakeAction());

	SoundManager::shareSoundManager().playEffect(GEM_TENLINE);
}

void FightScene::onHypercubeCombo()
{
	CCLog("hypercube combo");
	this->runAction(getShakeAction());

	SoundManager::shareSoundManager().playEffect(GEM_LIGHTBALL);
}

void FightScene::onUserHaveRemoveGemPassTime()
{
	if (m_pPropLayer)
	{
		m_pPropLayer->setShowHintPropBlink();
	}
}

void FightScene::onClearUserUserRemoveGemTime()
{
	m_recordUserRemoveGemTime = 0.f;
	m_lianjiSpaceTime = 0.f;
}

void FightScene::onGoldMineralExplosiveForAddGoldNum()
{
	if (m_pTopLayer)
	{
		int gold = getUserStoreData().getGold();
		gold += 10;
		m_pTopLayer->setUserGold(gold);
		getUserStoreData().setGold(gold);
		getUserStoreData().saveUserData();
	}
	
}

void FightScene::onMineralIsTopTheGameLose()
{
	onOver();
}

void FightScene::onNewWave(int cur, int total)
{
	if (m_pTopLayer)
	{
		m_pTopLayer->setWave(cur, total);
	}

 	CCSize size = CCDirector::sharedDirector()->getWinSize();
 	CCSprite* sprite = CCSprite::create("new_wave_2.png");
 	sprite->setPosition(ccp(size.width / 2, size.height / 2));
 	
 
	if (cur == total && m_nCopyId != 25)
	{

		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("lastWave.png");
		sprite->setTexture(texture);
	}
	this->addChild(sprite);

	CCFadeOut* fade = CCFadeOut::create(2.f);
	CCCallFuncND* callback_nd = CCCallFuncND::create(this, 
		callfuncND_selector(FightScene::cbDelNewWaveNode), NULL);
 	sprite->runAction(CCSequence::create(fade, callback_nd, NULL));
}

void FightScene::onMonsterAttackWall(MonsterNode* monster, int aggress)
{
	if (m_pWallLayer)
	{
		int rowIndex2 = -1;
		if (monster->getMonsterData()->isBoss()) {
			rowIndex2 = monster->getRowIndex() + 1;
		}
		m_pWallLayer->beAttacked(aggress, monster->getRowIndex(), rowIndex2);

		SoundManager::shareSoundManager().playEffect(MOSNTER_ATK_EFFECT);
	}
}

void FightScene::onMonsterLaunchSkillLetBoardShake()
{
	// �����𶯴���
	if (m_pBoardLayer)
	{
		this->runAction(getShakeAction());
	}
	
}

void FightScene::onMonsterOver() // �����ݽṹ�е����ݱ�����
{
	if (m_pMonsterLayer->getTag() == kInfinitiTag) // ���������ģʽ�Ļ������ٴμ�������
	{
		m_pMonsterLayer->loadData();
	}
	else
	{
		if (!m_bShowWinLayer)
		{
			gameWin();

			m_bShowWinLayer = true;
		}
	}
}

void FightScene::onAttackMonster(MonsterNode* node, int aggress, const std::vector<ConfigSpriteSkillStatusItem*>& vStatus)
{
	// �ͷż��ܹ������ﺯ��
// 	if (getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite) // ������ģʽ�Ļ������������������Ѷ�
// 	{
// 		aggress -= 50;
// 	}
	
	if (aggress != 0)
	{
		CCPoint nodePoint = node->getPosition();
		node->beAttacked(aggress, 900, 200, nodePoint.x, nodePoint.y, 4);
	}

	if (!vStatus.empty())
	{
		MonsterData* pMonsterData = const_cast<MonsterData*>(node->getMonsterData());
		if (pMonsterData)
		{
			std::vector<ConfigSpriteSkillStatusItem*>::const_iterator iter = vStatus.begin();
			for (; iter != vStatus.end(); ++iter)
			{
				pMonsterData->addStatus(*iter);
			}
		}
	}
}

void FightScene::onSceneStatus(const std::vector<ConfigSpriteSkillStatusItem*>& vStatus)
{
	if (!vStatus.empty())
	{
		std::vector<StatusData*> vStatusData;
		std::vector<ConfigSpriteSkillStatusItem*>::const_iterator iter = vStatus.begin();
		for (; iter != vStatus.end(); ++iter)
		{
			StatusData* data = new StatusData(*iter);
			vStatusData.push_back(data);
		}
		
		if (!vStatusData.empty())
		{
			SceneStatusData* pSceneStatusData = new SceneStatusData();
			pSceneStatusData->setSceneStatus(vStatusData);
			m_pMonsterLayer->addSceneStatus(pSceneStatusData);
		}
	}
}

void FightScene::onRecoveryHp(int recoveryHp)
{
	if (m_pWallLayer)
	{
		m_pWallLayer->recovery(recoveryHp);
	}
}


void FightScene::onIceSendToMonsterSkill(bool isBossSKill)
{
	if (m_pMonsterLayer)
	{
		m_pMonsterLayer->setIsBossSkillCanSend(isBossSKill);
	}
	
}
void FightScene::onSkillClick(SpriteSkillResult* result)     // ���ܱ�����¼�����
{
	if (m_pEffectLayer == NULL)
	{
		return;
	}

	EffectData* data = new EffectData();
	data->setResult(result);

	if (data->isGroup()) // �Ƿ���Ⱥ�幥��
	{
		data->setBeAttackedMonster(m_pMonsterLayer->getSkillGroupAttackMonster());
	}
	else
	{
		std::vector<MonsterNode*> vBeAttackedMonster;
		MonsterNode* pMonster = NULL;
		if (data->getActionType() == AT_QUALM) {
			pMonster = m_pMonsterLayer->getSkillDizzyMonster();
		}
		else {
			pMonster = m_pMonsterLayer->getSkillAttackMonster();
		}
		vBeAttackedMonster.push_back(pMonster);
		data->setBeAttackedMonster(vBeAttackedMonster);
	}

	m_pEffectLayer->runEffect(data);
}

bool FightScene::enableLaunch(int energy)
{
	if (m_pEnergyLayer)
	{
		return m_pEnergyLayer->subEnergy(energy);
	}
	return false;
}

int FightScene::onGetEnabel(int skillId) // ������Ǽ����Ƿ����ʹ�ã�����0��ʾ����ʹ�ã����ض�����ֵ��ʾ���ż�����ת�Ķ���
{
	switch (skillId)
	{
	case 15:
		if(m_pEnergyLayer->getCurEnerg() >= m_pEnergyLayer->getFullEnergyF()/4 )
		{
			return true;
		}
		break;
	case 6:
		if (m_pEnergyLayer->getCurEnerg() >= m_pEnergyLayer->getFullEnergyF()/2)
		{
			return true;
		}
		
		break;
	case 1:
		if (m_pEnergyLayer->getCurEnerg() >= m_pEnergyLayer->getFullEnergyF()/4*3)
		{
			return true;
		}
		break;
	case 9:
		if (m_pEnergyLayer->getCurEnerg() >= m_pEnergyLayer->getFullEnergyF())
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void FightScene::onEnergyChanged(int fullEnergy, int curEnergy) // �������۱仯��Ҫ�жϼ��ܵ��Ǹ���ת�������ż���
{
	if (m_pSkillLayer)
	{
		 m_pSkillLayer->countRunAnimateNum(fullEnergy, curEnergy);
	}
	
}

void FightScene::onWallHpChanged(int beforeChangedHp, int changedHp, int totalHp)
{
	if (m_pTopLayer)
	{
		m_pTopLayer->setHp(beforeChangedHp, changedHp, totalHp);
	}
}

void FightScene::onWallHpChange(float hp)
{
	if (m_pWallLayer)
	{
		m_pWallLayer->hpChangeForWallTexture(hp);
	}
	
}

void FightScene::onOver()
{
	if (!m_bShowLoseLayer)
	{
		gameLose();
		
		m_bShowLoseLayer = true;
	}
}

void FightScene::onUserGoldChanged(int gold)  // �޸��û����
{
	m_nCopyWinGold += gold;
	u32 userGold= getUserStoreData().getGold();
	userGold += gold;
	if (m_pTopLayer)
	{
		m_pTopLayer->setUserGold(userGold);
	}

	// �����û����� 
	getUserStoreData().setGold(userGold);
	getUserStoreData().saveUserData();
}

void FightScene::onMonsterLaunchShill(MonsterNode* node, const ConfigMonsterSkillItem* monsterSkill)
{
	// �ѹ����Ƿŵļ��ܴ������
	CCLog("MonsterLaunchSkill");
}

void FightScene::onLoseLayerRestartCopy()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	//CCDirector::sharedDirector()->resume();  // 
	if(m_pDelegate){
		m_pDelegate->onRestartCopy(m_nCopyId, m_isMineral);
	}
}

void FightScene::onLoseLyaerExitCopy()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	// CCDirector::sharedDirector()->resume(); // 
	CCDirector::sharedDirector()->popScene();
	// CCDirector::sharedDirector()->popToRootScene();
	if (m_pDelegate)
	{
		m_pDelegate->onFightExitForMenuLayer();
	}
}

void FightScene::onExitCopyClick()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	CCDirector::sharedDirector()->popScene();
	//CCDirector::sharedDirector()->popToRootScene();
	if (m_pDelegate)
	{
		m_pDelegate->onExitFightScene(m_nCopyId, m_isCopyUnLockBefore, m_isMineral, m_isNewUnlockMineral);
	}
}

void FightScene::onRestartCopyClick()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	if (m_pDelegate) {
		m_pDelegate->onRestartCopy(m_nCopyId, m_isMineral);
	}
}

void FightScene::onNextCopyClick()
{
	m_userScore = 0;
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	if (m_pDelegate) {
		m_pDelegate->onNextCopy(m_nCopyId);
	}
}

void FightScene::onMenuButtonClick()      // �����˵���ť����¼�����
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	m_pMenuLayer = MenuLayer::create();
	m_pMenuLayer->setPosition(CCPointZero);
	m_pMenuLayer->setDelegate(this);
	this->addChild(m_pMenuLayer, 20);
	CCDirector::sharedDirector()->pause();
}

void FightScene::onHelpMenuButtonClick()
{
 	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 	CCSprite* helpSprite = CCSprite::create("helpView.png");
	helpSprite->setTag(kHelpSpriteTag);
 	helpSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
	this->addChild(helpSprite);

	// ��Ϸ������ͣ״̬����û�취������
// 	CCParticleSystem* pParticle = CCParticleSystemQuad::create("sheng.plist");
// 	pParticle->setPosition(ccp(423, 147));
// 	pParticle->setPosition(ccp(353, 49));
// 	helpSprite->addChild(pParticle, 6);

	CCMenuItemImage* helpViewItem = CCMenuItemImage::create("helpViewItem.png", "helpViewItem.png",
		this, menu_selector(FightScene::helpViewItemCallback));
	helpViewItem->setPosition(CCPointZero);
	CCMenu* pHelpViewMenu = CCMenu::create(helpViewItem, NULL);
	pHelpViewMenu->setPosition(ccp(353, 49));
	helpSprite->addChild(pHelpViewMenu);
}

void FightScene::helpViewItemCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->resume();
	this->removeChildByTag(kHelpSpriteTag);
}
void FightScene::onMenuLayerResumeGame()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	if (m_pMenuLayer)
	{
		m_pMenuLayer->removeFromParent();
	}
}

void FightScene::onMenuLayerRestartGame()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	if (m_pDelegate) {
		m_pDelegate->onRestartCopy(m_nCopyId, m_isMineral);
	}
}

void FightScene::onMenuLayerExitGame()
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	CCDirector::sharedDirector()->popScene();
	// CCDirector::sharedDirector()->popToRootScene();
	if (m_pDelegate)
	{
		m_pDelegate->onFightExitForMenuLayer();
	}
	
}

void FightScene::onStoreButtonClick()
{
	u32 userGold = getUserStoreData().getGold();
	if (userGold >= 1000 )
	{
		m_pStoreLayer = StoreLayer::create();
		m_pStoreLayer->setDelegatewithFight(this);
		m_pStoreLayer->setThisStoreIsInFright(true);
		this->addChild(m_pStoreLayer, 3);
	}
	else
	{
		m_pFreeGoldLayer = FreeGoldLayer::create();
		m_pFreeGoldLayer->setAnchorPoint(CCPointZero);
		m_pFreeGoldLayer->setThisLayerInWhere(true);
		m_pFreeGoldLayer->setFightFreeeDelegate(this);
		m_pFreeGoldLayer->setPosition(ccp(0, 0));
		this->addChild(m_pFreeGoldLayer, 3);
	}
	
}

void FightScene::onStoreCancelCkickWithFight() // ��ս�������ڿ����̵꣬�̵��ȷ�ϰ�ť�ص�
{
	CCDirector::sharedDirector()->resume();
	if (m_pPropLayer)
	{
		m_pPropLayer->setPropNum();
	}
	if (m_pPropLayer)
	{
		m_pPropLayer->loadUserPropData();
	}

	// ���ý��
	u32 gold = getUserStoreData().getGold();
	if (m_pTopLayer)
	{
		m_pTopLayer->setUserGold(gold);
	}
}

void FightScene::onStoreForFreeChlick() // ��ս�����������ѽ�Ұ�ť
{
	m_pFreeGoldLayer = FreeGoldLayer::create();
	m_pFreeGoldLayer->setAnchorPoint(CCPointZero);
	m_pFreeGoldLayer->setThisLayerInWhere(true);
	m_pFreeGoldLayer->setFightFreeeDelegate(this);
	m_pFreeGoldLayer->setPosition(ccp(0, 0));
	this->addChild(m_pFreeGoldLayer, 3);
}

void FightScene::onFreeGoldLayerCancel()
{
	if (m_pFreeGoldLayer)
	{
		m_pFreeGoldLayer = NULL;
	}
	
}

void FightScene::onFreeLayerToStoreLayer() // ����ѽ�Ҵ����̵�
{
	if (m_pFreeGoldLayer)
	{
		m_pFreeGoldLayer = NULL;
	}
	m_pStoreLayer = StoreLayer::create();
	m_pStoreLayer->setDelegatewithFight(this);
	m_pStoreLayer->setThisStoreIsInFright(true);
	this->addChild(m_pStoreLayer, 3);
}

void FightScene::onPropClick(int nPropId)				 // ���߱����
{
	SoundManager::shareSoundManager().playEffect(BUTTON_CLINK_MUSIC);
	u32 num = getUserStoreData().getPropNum(nPropId);    // ��ȡ�û������id��Ӧ�ĵ�������
	if (num > 0) 
	{
		getUserStoreData().setPropNum(nPropId, --num);   // ���û������id��Ӧ�ĵ���������ȥ1
		getUserStoreData().saveUserData();               // �����û�������
		propClickEffect(nPropId);
	}
	else if(num == 0)
	{
		int gold = getUserStoreData().getGold();
		const ConfigPropItem* propItem = getConfigProp().getConfigPropItem(nPropId);
		int propPrice = propItem->price;
		if (gold <  propPrice)
		{
			// ����û��Ľ������С�ڵ��ߵļ۸�ѯ���û��Ƿ����ѽ���̵�
			CCSprite* askView = CCSprite::create("askview.png");
			askView->setPosition(ccp(480, 320));
			askView->setTag(kAskViewTag);
			this->addChild(askView);

			CCMenuItemImage* itemOk = CCMenuItemImage::create("shiUp.png", "shiDown.png",
				this,  menu_selector(FightScene::onUserClickOk));
			itemOk->setPosition(CCPointZero);
			CCMenu* menuOk = CCMenu::create(itemOk, NULL);
			menuOk->setPosition(ccp(183, 24));
			askView->addChild(menuOk);

			CCMenuItemImage* itemCancel = CCMenuItemImage::create("foUp.png", "foDown.png", 
				this, menu_selector(FightScene::onUserClickCancel));
			itemCancel->setPosition(CCPointZero);
			CCMenu* menuCancel = CCMenu::create(itemCancel,NULL);
			menuCancel->setPosition(ccp(519, 21));
			askView->addChild(menuCancel);
			CCDirector::sharedDirector()->pause();

		}
		else
		{
			gold = (gold - propPrice);
			getUserStoreData().setGold(gold);
			if (m_pTopLayer)
			{
				m_pTopLayer->setUserGold(gold);
			}
			getUserStoreData().saveUserData();  
			propClickEffect(nPropId);
		}
	}
	
}

void FightScene::onUserClickOk(CCObject* pSender)
{
	onStoreButtonClick();

	CCNode* sprite = this->getChildByTag(kAskViewTag);
	if (sprite)
	{
		sprite->removeFromParent();
	}
}
void FightScene::onUserClickCancel(CCObject* pSender)
{
	CCNode* sprite = this->getChildByTag(kAskViewTag);
	if (sprite)
	{
		sprite->removeFromParent();
	}
	CCDirector::sharedDirector()->resume();
}

void FightScene::onFirstPropShanCloseSendToFight()
{
	if (m_pBoardLayer)
	{
		m_pBoardLayer->setUserRestRemoveTime(0.f);
	}
}

bool FightScene::onIsUserRemovegemNow() 
{
	if (m_pBoardLayer)
	{
		return (m_pBoardLayer->getIsRemoveGemNow());
	}
}

void FightScene::propClickEffect(int nPropId)
{
	if (nPropId == kPropHint) {                      // ����ǷŴ�
		m_pBoardLayer->showHint();
	}
	else if (nPropId == kPropEnergy) {               // ���������
		const ConfigPropItem* pCfgPropItem = getConfigProp().getConfigPropItem(nPropId);
		if (pCfgPropItem) {
			m_pEnergyLayer->addEnergy(pCfgPropItem->param);
		}
	}
	else if (nPropId == kPropRefreshBoard) {        // �����ˢ��
		m_pBoardLayer->refresh();
		if (this->getChildByTag(kHinitTag))
		{
			this->getChildByTag(kHinitTag)->stopAllActions();
			this->removeChildByTag(kHinitTag);
		}
	}
	else if (nPropId == kPropMushroom) {      
		// ���������Ģ��
		m_pBoardLayer->randConvertToMushroom();
		if (this->getChildByTag(kHinitTag))
		{
			this->getChildByTag(kHinitTag)->stopAllActions();
			this->removeChildByTag(kHinitTag);
		}
	}
}

CCActionInterval* FightScene::getShakeAction()
{
	CCActionInterval* move11 = CCMoveBy::create(0.015f, ccp(-6, -10));
	CCActionInterval* move12 = CCMoveBy::create(0.03f, ccp(12, 20));
	CCActionInterval* move13 = CCMoveBy::create(0.03f, ccp(-12, -20));
	CCActionInterval* move14 = CCMoveBy::create(0.015f, ccp(6, 10));

	CCActionInterval* move21 = CCMoveBy::create(0.015f, ccp(6, -10));
	CCActionInterval* move22 = CCMoveBy::create(0.03f, ccp(-12, 20));
	CCActionInterval* move23 = CCMoveBy::create(0.03f, ccp(12, -20));
	CCActionInterval* move24 = CCMoveBy::create(0.015f, ccp(-6, 10));
	return CCRepeat::create(CCSequence::create(move11, move12, move13, move14, move21, move22, move23, move24, NULL), 3);
}

std::string FightScene::getParticleSystemFileName(GemCellRemoveType removeType)
{ 
	if (removeType == GCRT_NORMAL) {
		return "fire.plist";
	}
	else if (removeType == GCRT_HYPERCUBE) {
		return "guang.plist";
	}
	else if (removeType == GCRT_EXPLOSIVE) {
		return "fire.plist";
	}
	else if (removeType == GCRT_ROWCOLUMNREMOVE) {
		return "ice.plist";
	}
	return "fire.plist";
}

void FightScene::cbMoveBulletEnd(CCNode* node, void* data)    // �����ƶ����
{
	node->removeFromParent();

	CallbackData* callback_data = reinterpret_cast<CallbackData*>(data);
	MonsterNode* pMonsterNode = reinterpret_cast<MonsterNode*>(callback_data->data1);
	int aggress = reinterpret_cast<int>(callback_data->data2);
	int beginX = reinterpret_cast<int>(callback_data->data3);
	int beginY = reinterpret_cast<int>(callback_data->data4);
	int endX = reinterpret_cast<int>(callback_data->data5);
	int endY = reinterpret_cast<int>(callback_data->data6);
	int nMonsterDieJumpNum = reinterpret_cast<int>(callback_data->data7);

	pMonsterNode->beAttacked(aggress,beginX, beginY, endX, endY, nMonsterDieJumpNum);
}

void FightScene::cbDelNewWaveNode(CCNode* node, void* data)
{
	node->removeFromParent();

	m_pMonsterLayer->nextWave();
}

void FightScene::gameLose()
{
	if (m_nCopyId == 25) // ���������ģʽ�Ļ����˾͵�ʤ����
	{
		gameWin();
	}
	else
	{
		this->cleanup();

		LoseLayer* layer = LoseLayer::create();
		layer->setPosition(CCPointZero);
		layer->setDelegate(this);
		this->addChild(layer, 20);

		SoundManager::shareSoundManager().playEffect(FAIL_MUSIC);	
	}	
}

void FightScene::gameWin()
{
	// m_pWallLayer->m_nCurHp; // ��ǰ��Ѫ�� 
	// m_pTopLayer->m_fDeltaTime; // ��ȡ���˵�ǰ���õ�ʱ�� 
	u32 starNum = base::getWinStarNum(m_pWallLayer->m_nCurHp, m_pTopLayer->m_fDeltaTime, m_nCopyId); 
	if (m_isMineral)
	{
		bool value = getUserStoreData().getCopyMineral(m_nCopyId);
		if (!value)
		{
			getUserStoreData().setCopyMineral(m_nCopyId, true);
			m_isNewUnlockMineral = true;
		}
	}
	
	int infinite = getConfigCopy().getConfigCopyItem(m_nCopyId)->infinite; 
	WinLayer* layer = WinLayer::create(starNum, m_userScore, infinite); //  ������ ������  ���� �Ƿ�������ģʽ 
	layer->setPosition(CCPointZero); 
	layer->setDelegate(this); 
	this->addChild(layer, 20); 
	
	// TODO: �洢�û�����
	// ���� 
	int usercopyScore = getUserStoreData().getScoreWithCopy(m_nCopyId);
	if (m_userScore >= usercopyScore)
	{
		getUserStoreData().setScoreWithCopy(m_nCopyId, m_userScore);
		CCSprite* sprite = CCSprite::create("newScore.png");
		sprite->setPosition(ccp(-130, 470));
		layer->addChild(sprite, 2);

		CCDelayTime* delay = CCDelayTime::create(1.f);
		CCActionInterval *move = CCMoveTo::create(0.3f, ccp(330, 270));
		CCEaseExponentialIn* ease = CCEaseExponentialIn::create(move);
		CCSequence* action = CCSequence::create(delay, ease, NULL);
		sprite->runAction(action);
	}
	// ����
	int beforScore = getUserStoreData().getScore(m_nCopyId);
	if (beforScore != 0)
	{
		m_isCopyUnLockBefore =true;
	}
	if (beforScore <= starNum)
	{
		getUserStoreData().setScore(m_nCopyId, starNum);
	}
	getUserStoreData().saveUserData();

	SoundManager::shareSoundManager().playEffect(GET_COIN_MUSIC); 

	if (m_pDelegate) {
		m_pDelegate->onWin(m_nCopyId);
	}
	m_pTopLayer->stopUpdate(); //����ʱ���ʱ��
}

void FightScene::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, false);
}

bool FightScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)// ����ʵ��virtual void ccTouchMoved
{
	return false;
}

