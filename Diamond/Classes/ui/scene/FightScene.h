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
	// 安卓代理 
	virtual void back();
	// override BoardLayerDelegate
	// 棋盘上有产生消除
	virtual void onCombo(int combo);			
	// 棋盘珠子爆炸
	// |first|爆炸类型，|second|爆炸的珠子
	virtual void onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem, int boardSendToFightAttackNum);
	// 棋盘不能移动 
	virtual void onBoardImmobile(); 
	// 9宫格  
	virtual void onExplosiveCombo();		 
	// 10字消除  
	virtual void onRowColumnRemoveCombo();	 	
	// 同色消除 
	virtual void onHypercubeCombo();	
	// 当用户没有消除珠子超过规定的时间，就让放大镜道具提示 
	virtual void onUserHaveRemoveGemPassTime();
	// 爆炸动作之前，清除用户的连击操作时间 
	virtual void onClearUserUserRemoveGemTime(); 
	// 当处于矿石模式的时候，矿石挤满棋盘，游戏结束
	virtual void onMineralIsTopTheGameLose();
	// 当金矿被炸，要增加金币 
	virtual void onGoldMineralExplosiveForAddGoldNum();
	////////////////////////////////////////////////////////////////////////
	// override MonsterLayerDelegate
	// 新的一波|cur|当前波,|total|总波 
	virtual void onNewWave(int cur, int total);     
	// 怪物攻击围栏
	virtual void onMonsterAttackWall(MonsterNode* monster, int aggress); 
	// 当前副本的怪物全部死亡
	virtual void onMonsterOver();
	virtual void onUserGoldChanged(int gold); // 当怪被打死后，需要修改用户的金币
	virtual void onMonsterLaunchShill(MonsterNode* node, const ConfigMonsterSkillItem* monsterSkill);
	// 怪物释放技能条导致棋盘震动
	virtual void onMonsterLaunchSkillLetBoardShake();
	////////////////////////////////////////////////////////////////////////
	// override  EffectLayerDelegate
	// 技能怪物攻击
	// |node|怪物对象，|aggress|技能对怪物产生的攻击伤害，|vStatus|该技能的副带状态
	virtual void onAttackMonster(MonsterNode* node, int aggress, const std::vector<ConfigSpriteSkillStatusItem*>& vStatus);
	// 状态附加给场景的状态，如：场景上结一层冰
	virtual void onSceneStatus(const std::vector<ConfigSpriteSkillStatusItem*>& vStatus);
	// 回血技能
	virtual void onRecoveryHp(int recoveryHp);

	virtual void onIceSendToMonsterSkill(bool isBossSKill);

	////////////////////////////////////////////////////////////////////////
	// override  SkillLayerDelegate
	// 技能被点击事件处理
	virtual void onSkillClick(SpriteSkillResult* result);    
	// 能量糟能量是否足够
	virtual bool enableLaunch(int energy);
	virtual int onGetEnabel(int skillId);

	////////////////////////////////////////////////////////////////////////
	// override  EnergyLayerDelegate
	// 能量糟能量发生变化
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
	// 围栏的hp发生变化
	virtual void onWallHpChanged(int beforeChangedHp, int changedHp, int totalHp);
	// 围栏的hp值为0
	virtual void onOver();
	virtual void onWallHpChange(float hp);

	////////////////////////////////////////////////////////////////////////
	// override  LoseLayerDelegate
	// 游戏失败层的重玩按钮处理
	virtual void onLoseLayerRestartCopy();    
	// 游戏失败层的退出按钮
	virtual void onLoseLyaerExitCopy();       

	////////////////////////////////////////////////////////////////////////
	// override  WinLayerDelegate
	// 下一关点击事件处理 
	virtual void onNextCopyClick();
	// 重新玩 点击事件处理 
	virtual void onRestartCopyClick();
	// 退出事件处理 
	virtual void onExitCopyClick();

	////////////////////////////////////////////////////////////////////////
	// override  TopLayerDelegate
	// 菜单按钮点击事件
	virtual void onMenuButtonClick();  
	virtual void onHelpMenuButtonClick();

	////////////////////////////////////////////////////////////////////////
	// override  MenuLayerDelegate
	// 游戏继续按钮事件处理 
	virtual void onMenuLayerResumeGame();
	// 游戏重新开始按钮事件处理 
	virtual void onMenuLayerRestartGame();
	// 游戏退出按钮事件处理 
	virtual void onMenuLayerExitGame();
	virtual void onStoreButtonClick();

	////////////////////////////////////////////////////////////////////////
	// override  PropLayerDelegate
	// 道具点击事件处理 
	virtual void onPropClick(int nPropId);   
	// 放大镜上面的闪烁效果因为用户点击放大镜关闭，需要通知计时器重新开始计时 
	virtual void onFirstPropShanCloseSendToFight();
	// 查看用户是否正在消除珠子，如果是的话要让放大镜失效，否者有bug 
	virtual bool onIsUserRemovegemNow();

	/////////////////////////////////////////////////////////////////////////
	// 道具被点击的效果
	void propClickEffect(int nPropId); 
	// 帮组界面确认按钮
	void helpViewItemCallback(CCObject* pSender);
private:
	CCActionInterval* getShakeAction();  // 整个战斗场景震动
	std::string getParticleSystemFileName(GemCellRemoveType removeType);
	void cbBoardImmobileHint(CCNode* node, void* data);

private:
	void onUserClickOk(CCObject* pSender);
	void onUserClickCancel(CCObject* pSender);

private:
	void cbMoveBulletEnd(CCNode* node, void* data);
	void cbDelNewWaveNode(CCNode* node, void* data);

	void gameLose();  // 游戏失败
	void gameWin();   // 游戏胜利
	virtual void registerWithTouchDispatcher();

	void setAutoRemoveGemVisible();
	void setUserRemoveGemVisible();
	void cbAutoRemoveGemEffect(CCNode* node, void* data);

	// 创建连击提示
	void createLianJiEfffct();

private:
	MonsterLayer* m_pMonsterLayer;  // 怪物层
	SkillLayer* m_pSkillLayer;      // 技能层
	EffectLayer* m_pEffectLayer;    // 特效层    展现技能的效果
	EnergyLayer* m_pEnergyLayer;    // 能量层
	TopLayer* m_pTopLayer;          // 顶层
	WallLayer* m_pWallLayer;        // 栅栏层
	MenuLayer* m_pMenuLayer;        // 菜单层
	BoardLayer* m_pBoardLayer;      // 棋盘层
	StoreLayer* m_pStoreLayer;      // 商店层
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

	// 用户的分数要乘以的倍数
	float m_userScoreMultiple;
	float m_userScore;

	// 连击数据控制数据 
	float m_RecordAutoRemoveGemTime;
	float m_recordUserRemoveGemTime;
	int m_RecordAutoRemoveGemNum;
	int m_recordUserRemoveGemNum;
	bool m_AutoUpdateRecord;

	// 用力判断是否可以提示连击
	int m_lianjiCount; 
	float m_lianjiSpaceTime;
	bool m_AutoIsCanVisible;

	bool m_lianjiMaxClose;
	float m_lianjiMaxCloseTime;

	// 用来判断之前是否是解锁过的关卡
	bool m_isCopyUnLockBefore;

	// 用来标记是否是矿石模式
	bool m_isMineral;
	bool m_isNewUnlockMineral;
private:
	DISALLOW_COPY_AND_ASSIGN(FightScene);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);// 必须实现virtual void ccTouchMoved

};

#endif  // _UI_SCENE_MAIN_SCENE_H_