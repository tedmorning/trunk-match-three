#ifndef _UI_SCENE_FIGHT_SCENE_H_
#define _UI_SCENE_FIGHT_SCENE_H_

#include "cocos2d.h"
#include "MonsterLayer.h"
#include "BoardLayer.h"
#include "SkillLayer.h"
#include "EffectLayer.h"
#include "EnergyLayer.h"
#include "TopLayer.h"
#include "WallLayer.h"
#include "LoseLayer.h"
#include "WinLayer.h"
#include "MenuLayer.h"
#include "PropLayer.h"
#include "StoreLayer.h"
#include "FreeGoldLayer.h"
#include "androidDelegate.h"

USING_NS_CC;

class FightSceneDelegate
{
public:
	virtual void onExitFightScene(int nCopyId, bool isCopyUnlock, bool isMineral = false, bool isMineralNewUnlock = false){}
	virtual void onWin(int nCopyId) {}
	virtual void onLose(int nCopyId) {}
	virtual void onNextCopy(int nCopyId) {}
	virtual void onRestartCopy(int nCopyId, bool isMineral) {}
//	virtual void onMonsterBeAttackSendPointToMonsterNode(CCPoint* begin, CCPoint* end){}
	virtual void onFightExitForMenuLayer(){}
	

protected:
	virtual ~FightSceneDelegate() {}
};

class FightScene : public CCLayer
	, public BoardLayerDelegate
	, public MonsterLayerDelegate
	, public SkillLayerDelegate
	, public EffectLayerDelegate
	, public EnergyLayerDelegate
	, public WallLayerDelegate
	, public LoseLayerDelegate
	, public WinLayerDelegate
	, public TopLayerDelegate
	, public MenuLayerDelegate
	, public PropLayerDelegate
	, public storeLayerDelegateWithFight
	, public ToFightSceneDelegate
	, public AndroidDelegate
{
public:
	FightScene(int nCopyId, bool isMineral);
	~FightScene();
	
public:
	static CCScene* scene(int nCopyId, bool isMineral, FightSceneDelegate* delegate = NULL);
	static FightScene* create(int nCopyId, bool isMineral, FightSceneDelegate* delegate);

public:
	void setDelegate(FightSceneDelegate* delegate) { m_pDelegate = delegate; }

protected:
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	void update(float delta);

	////////////////////////////////////////////////////////////////////////
	// ��׿���� 
	virtual void back();
	// override BoardLayerDelegate
	// �������в�������
	virtual void onCombo(int combo);			
	// �������ӱ�ը
	// |first|��ը���ͣ�|second|��ը������
	virtual void onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem, int boardSendToFightAttackNum);
	// ���̲����ƶ� 
	virtual void onBoardImmobile(); 
	// 9����  
	virtual void onExplosiveCombo();		 
	// 10������  
	virtual void onRowColumnRemoveCombo();	 	
	// ͬɫ���� 
	virtual void onHypercubeCombo();	
	// ���û�û���������ӳ����涨��ʱ�䣬���÷Ŵ󾵵�����ʾ 
	virtual void onUserHaveRemoveGemPassTime();
	// ��ը����֮ǰ������û�����������ʱ�� 
	virtual void onClearUserUserRemoveGemTime(); 
	// �����ڿ�ʯģʽ��ʱ�򣬿�ʯ�������̣���Ϸ����
	virtual void onMineralIsTopTheGameLose();
	// �����ը��Ҫ���ӽ�� 
	virtual void onGoldMineralExplosiveForAddGoldNum();
	////////////////////////////////////////////////////////////////////////
	// override MonsterLayerDelegate
	// �µ�һ��|cur|��ǰ��,|total|�ܲ� 
	virtual void onNewWave(int cur, int total);     
	// ���﹥��Χ��
	virtual void onMonsterAttackWall(MonsterNode* monster, int aggress); 
	// ��ǰ�����Ĺ���ȫ������
	virtual void onMonsterOver();
	virtual void onUserGoldChanged(int gold); // ���ֱ���������Ҫ�޸��û��Ľ��
	virtual void onMonsterLaunchShill(MonsterNode* node, const ConfigMonsterSkillItem* monsterSkill);
	// �����ͷż���������������
	virtual void onMonsterLaunchSkillLetBoardShake();
	////////////////////////////////////////////////////////////////////////
	// override  EffectLayerDelegate
	// ���ܹ��﹥��
	// |node|�������|aggress|���ܶԹ�������Ĺ����˺���|vStatus|�ü��ܵĸ���״̬
	virtual void onAttackMonster(MonsterNode* node, int aggress, const std::vector<ConfigSpriteSkillStatusItem*>& vStatus);
	// ״̬���Ӹ�������״̬���磺�����Ͻ�һ���
	virtual void onSceneStatus(const std::vector<ConfigSpriteSkillStatusItem*>& vStatus);
	// ��Ѫ����
	virtual void onRecoveryHp(int recoveryHp);

	virtual void onIceSendToMonsterSkill(bool isBossSKill);

	////////////////////////////////////////////////////////////////////////
	// override  SkillLayerDelegate
	// ���ܱ�����¼�����
	virtual void onSkillClick(SpriteSkillResult* result);    
	// �����������Ƿ��㹻
	virtual bool enableLaunch(int energy);
	virtual int onGetEnabel(int skillId);

	////////////////////////////////////////////////////////////////////////
	// override  EnergyLayerDelegate
	// ���������������仯
	virtual void onEnergyChanged(int fullEnergy, int curEnergy);

	////////////////////////////////////////////////////////////////////////
	// coerride storedelegate
	virtual void onStoreCancelCkickWithFight();
	virtual void onStoreForFreeChlick();

	// override ToFightSceneDelegate
	virtual void onFreeLayerToStoreLayer();
	virtual void onFreeGoldLayerCancel();

	////////////////////////////////////////////////////////////////////////
	// override  WallLayerDelegate
	// Χ����hp�����仯
	virtual void onWallHpChanged(int beforeChangedHp, int changedHp, int totalHp);
	// Χ����hpֵΪ0
	virtual void onOver();
	virtual void onWallHpChange(float hp);

	////////////////////////////////////////////////////////////////////////
	// override  LoseLayerDelegate
	// ��Ϸʧ�ܲ�����水ť����
	virtual void onLoseLayerRestartCopy();    
	// ��Ϸʧ�ܲ���˳���ť
	virtual void onLoseLyaerExitCopy();       

	////////////////////////////////////////////////////////////////////////
	// override  WinLayerDelegate
	// ��һ�ص���¼����� 
	virtual void onNextCopyClick();
	// ������ ����¼����� 
	virtual void onRestartCopyClick();
	// �˳��¼����� 
	virtual void onExitCopyClick();

	////////////////////////////////////////////////////////////////////////
	// override  TopLayerDelegate
	// �˵���ť����¼�
	virtual void onMenuButtonClick();  
	virtual void onHelpMenuButtonClick();

	////////////////////////////////////////////////////////////////////////
	// override  MenuLayerDelegate
	// ��Ϸ������ť�¼����� 
	virtual void onMenuLayerResumeGame();
	// ��Ϸ���¿�ʼ��ť�¼����� 
	virtual void onMenuLayerRestartGame();
	// ��Ϸ�˳���ť�¼����� 
	virtual void onMenuLayerExitGame();
	virtual void onStoreButtonClick();

	////////////////////////////////////////////////////////////////////////
	// override  PropLayerDelegate
	// ���ߵ���¼����� 
	virtual void onPropClick(int nPropId);   
	// �Ŵ��������˸Ч����Ϊ�û�����Ŵ󾵹رգ���Ҫ֪ͨ��ʱ�����¿�ʼ��ʱ 
	virtual void onFirstPropShanCloseSendToFight();
	// �鿴�û��Ƿ������������ӣ�����ǵĻ�Ҫ�÷Ŵ�ʧЧ��������bug 
	virtual bool onIsUserRemovegemNow();

	/////////////////////////////////////////////////////////////////////////
	// ���߱������Ч��
	void propClickEffect(int nPropId); 
	// �������ȷ�ϰ�ť
	void helpViewItemCallback(CCObject* pSender);
private:
	CCActionInterval* getShakeAction();  // ����ս��������
	std::string getParticleSystemFileName(GemCellRemoveType removeType);
	void cbBoardImmobileHint(CCNode* node, void* data);

private:
	void onUserClickOk(CCObject* pSender);
	void onUserClickCancel(CCObject* pSender);

private:
	void cbMoveBulletEnd(CCNode* node, void* data);
	void cbDelNewWaveNode(CCNode* node, void* data);

	void gameLose();  // ��Ϸʧ��
	void gameWin();   // ��Ϸʤ��
	virtual void registerWithTouchDispatcher();

	void setAutoRemoveGemVisible();
	void setUserRemoveGemVisible();
	void cbAutoRemoveGemEffect(CCNode* node, void* data);

	// ����������ʾ
	void createLianJiEfffct();

private:
	MonsterLayer* m_pMonsterLayer;  // �����
	SkillLayer* m_pSkillLayer;      // ���ܲ�
	EffectLayer* m_pEffectLayer;    // ��Ч��    չ�ּ��ܵ�Ч��
	EnergyLayer* m_pEnergyLayer;    // ������
	TopLayer* m_pTopLayer;          // ����
	WallLayer* m_pWallLayer;        // դ����
	MenuLayer* m_pMenuLayer;        // �˵���
	BoardLayer* m_pBoardLayer;      // ���̲�
	StoreLayer* m_pStoreLayer;      // �̵��
	PropLayer* m_pPropLayer;
	FreeGoldLayer* m_pFreeGoldLayer;

	FightSceneDelegate* m_pDelegate;

	bool m_bShowWinLayer;
	bool m_bShowLoseLayer;


	int m_nCopyId;
	int m_nCopyWinGold;

	//////////////////////////////////////////////////////
	int m_beginX;
	int m_beginY;
	int m_endX;
	int m_endY;
	int m_nMonsterDieJumpNum;

	int m_delayTime;
	bool m_isParticleCreate;
	bool m_isFreeGoldLayerToCreateStore;

	// �û��ķ���Ҫ���Եı���
	float m_userScoreMultiple;
	float m_userScore;

	// �������ݿ������� 
	float m_RecordAutoRemoveGemTime;
	float m_recordUserRemoveGemTime;
	int m_RecordAutoRemoveGemNum;
	int m_recordUserRemoveGemNum;
	bool m_AutoUpdateRecord;

	// �����ж��Ƿ������ʾ����
	int m_lianjiCount; 
	float m_lianjiSpaceTime;
	bool m_AutoIsCanVisible;

	bool m_lianjiMaxClose;
	float m_lianjiMaxCloseTime;

	// �����ж�֮ǰ�Ƿ��ǽ������Ĺؿ�
	bool m_isCopyUnLockBefore;

	// ��������Ƿ��ǿ�ʯģʽ
	bool m_isMineral;
	bool m_isNewUnlockMineral;
private:
	DISALLOW_COPY_AND_ASSIGN(FightScene);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);// ����ʵ��virtual void ccTouchMoved

};

#endif  // _UI_SCENE_MAIN_SCENE_H_