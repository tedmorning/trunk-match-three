#ifndef _UI_LAYER_BOARD_LAYER_H_
#define _UI_LAYER_BOARD_LAYER_H_

#include <map>
#include "cocos2d.h"
#include "GemCellEx.h"
/*#include "BaseLayer.h"*/
USING_NS_CC;
// combo是照成消除的意思

class BoardLayerDelegate
{
public:
	virtual void onCombo(int combo) {}
	virtual void onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem, int boardSendToFightAttackNum) {}
	virtual void onBoardImmobile() {}
	virtual void onExplosiveCombo() {}
	virtual void onRowColumnRemoveCombo() {}
	virtual void onHypercubeCombo() {}
	virtual void onUserHaveRemoveGemPassTime(){}
	virtual void onClearUserUserRemoveGemTime(){}
	virtual void onMineralIsTopTheGameLose(){}
	virtual void onGoldMineralExplosiveForAddGoldNum(){}

protected:
	virtual ~BoardLayerDelegate() {}
};

class BoardLayer : public CCLayer, public GemCellExDelegate
{
	// 消除方向
	enum ComboDirection
	{
		COMBO_ROW,
		COMBO_COLUMN,
	};

	enum LightingType
	{
		LT_HORIZONAL,
		LT_VERTICAL,
	};

public:
	BoardLayer(int gemTypeNum, bool isMineral);
	~BoardLayer();

public:
	void setDelegate(BoardLayerDelegate* delegate) { m_pDelegate = delegate; }

public:
	void start();
	void refresh();
	void newGame();
	void showHint();
	void randConvertToMushroom();

	static BoardLayer* create(int gemTypeNum, bool isMineral);

protected:
	virtual bool init();
	virtual void update(float dt);

protected:
	////////////////////////////////////////////////////////////////////////////
	// override GemCellExDelegate
	virtual void onTouchDown(Button* button);                      // 按下
	virtual void onTouchDrag(GemCellEx* gem, DragDirection dire);  // 滑动
	virtual void onGemSelected(GemCellEx* gem);             // 选中珠子
	virtual void onGemUnselected(GemCellEx* gem);           // 取消选中珠子
	virtual void onGemExplosiveModifier(GemCellEx* gem);    // 爆炸消除
	virtual void onGemRowColumnRemoveModifier(GemCellEx* gem);  // 行列消除
	virtual void onGemHypercubeModifier(GemCellEx* gem);        // 同色消除
	 
private:
	////////////////////////////////////////////////////////////////////////////
	// Action
	void gemMoveAction(GemCellEx* gem, const CCPoint& pt, float time = 0.5f, CCCallFuncND* callback = NULL);
	void gemMoveAndReverseAction(GemCellEx* gem, const CCPoint& pt);

	void shuffleDown();
	void shuffleAllDown();
	void fillBoard();
	void shuffleDown(const std::vector<GemCellEx*>& vGem, float time = 0.5f);

	void exchangeGemAndReverseAction(GemCellEx* gem1, GemCellEx* gem2); // 不能滑动的珠子的回复原位动作
	void exchangeGemAction(GemCellEx* gem1, GemCellEx* gem2); 
	void exchangeAction(GemCellEx* gem1, GemCellEx* gem2);   // 交换珠子的动作

	void hideHint();

	// NOTE: 
	// 1、十字消、爆炸、同色消除的珠子被消除引起爆炸的动画，0.9秒，
	// 2、动画播放结束后，进行消除判断，如果是爆炸，则会引起上面的珠子上下震动一下

	// 十字消、爆炸、同色消除的珠子被消除引起爆炸的动画

	float beforeExplosiveAction(); // 爆炸之前的特效

	// NOTE:
	// COMBO：计算方式：
	// 多少时间内没有滑动珠子，则combo重新开始
	// 如果在interval的时间的有产生消除的情况下（人为或者自动消除），则combo数量加上去
	// float m_fComboDeltaTime;
	// int m_nCombo;
	// virtual void update(float dt) { if (m_fComboDeltaTime > value) { m_nCombo = 0; } else { m_fComboDeltaTime += dt; } }

	////////////////////////////////////////////////////////////////////////////
	// Action Effect
	// NOTE: 这块临时这样写，后面有机会再做封装
	CCActionInterval* createExplosiveAction();
	CCActionInterval* createRowColumnRemoveAction(LightingType direction);
	CCActionInterval* createHypercubeAction();

	////////////////////////////////////////////////////////////////////////////
	// callback
	void cbGemMoveAction(CCNode* node, void* data);				// 珠子移动动作
	void cbRemoveGemAction(CCNode* node, void* data);			// 删除珠子动作
	void cbRemoveGemActionForMineral(CCNode* node, void* data);			// 删除珠子动作
	void cbChangeGemModifierAction(CCNode* node, void* data);	// 
	void cbShuffleDownAction(CCNode* node, void* data);
	void cbFillBoardAction(CCNode* node, void* data);
	void cbMarkComboAction(CCNode* node, void* data);
	void cbExplosiveActionEnd(CCNode* node, void* data);
	void cbBeginExplosiveAction(CCNode* node, void* data);
	void cbGemMoveActionCallback(CCNode* node, void* data);
	void goldMineralExplosiveCallback(CCNode* node, void* data);// 石头爆炸的callback
	void soilMineralExplisiveCallback(CCNode* node, void* data);
	////////////////////////////////////////////////////////////////////////////
	// update
	void updateCombo(float dt);
	void updateImmobile(float dt);
	void updateUserRestTime(float dt);
	void updateMineral(float dt);

private:
	void clearBoard();
	void resetBoard();
	// 插入一行矿石 
	void boardAddMineral();
	// 计算矿石模式爆掉的珠子
	void addMinderalGem(int row, int column, std::vector<GemCellEx*>* vMineral, bool isNeedTwice = true, bool isRowColumn = false);

	void selectedGem(GemCellEx* gem);
	void deselectCurrentGem();

	bool cellInBoard(int row, int column);

	GemCellEx* createMineralBlock(int row, int column, int starRow, bool isInsertVector = false); // 创建一颗矿石珠子
	GemCellEx* createBlock(int row, int column, int startRow); // 创建一颗珠子 
	GemCellEx* safeGetCell(int row, int column); 
	GemCellEx* safeGetCell(unsigned int index); 

	ELE_TYPE safeGetCellType(int row, int column);
	CCPoint getGemPosition(int row, int column);

	int index(int row, int column) const;
	// 返回棋盘数组中的一颗珠子
	GemCellEx* board(int row, int column);
	void setCell(int row, int column, GemCellEx* value, bool isInsertVector = false);
	void exchangeGemCellPos(GemCellEx* gem1, GemCellEx* gem2);

	ELE_TYPE generateGemCellType();
	// 随机生成一种矿石模式的珠子
	ELE_TYPE generateGemCellMineralType();

	bool hyperCubeUsed();
	bool bothHyperCube();
	bool enableExchange();
	GemCellEx* getSideGemCell(DragDirection dire);

	bool findCombo();
	bool findComboInLine(int line, ComboDirection direction);
	bool findCombosIfSwitchGem(GemCellEx* gem1, GemCellEx* gem2);

	bool markCombo();
	bool markComboInLine(int line, ComboDirection direction);

	void markIntersections();
	void markBonusGems();
	void markExplosions();

	void explodeGem(int row, int column, GemCellRemoveType removeType = GCRT_NORMAL);
	bool hasRowCombo(int row, int column);
	bool hasColumnCombo(int row, int column);

	void removeAll();
	void removeCombo();

	// 是否需要给棋盘添加珠子
	bool isNeedShuffleDown();
	bool isNeedFillBoard();

	void resetInvincibleStatus();
	void resetComboDeltaTime();
	void addCombo();

	void swapGem(GemCellEx* gem1, GemCellEx* gem2); // 交换两颗珠子的位置
	bool hasPossibleCombos(int* hintIdx);

	CCPoint calcLightingSpritePos(GemCellEx* gem, LightingType direction);
	std::string getExplosiveFile(ELE_TYPE eleType);            
	std::string getRowColumnRemoveFile(ELE_TYPE eleType);

public:
		void setUserRestRemoveTime(float time){ m_UserHaveNotRemoveGemTime = time;}
		bool isUserMoveHaveonCombo(){ return m_IsUserMoveGem; }
		void setUserMoveGem(bool value){ m_IsUserMoveGem = value; }

		bool getIsRemoveGemNow(){ return m_IsRemoveGemNow; }
private:
	CCActionInterval* getStonrExplosiveAnimate();
	CCActionInterval* getGoldExplosiveAnimate();
	CCActionInterval* getSoilExplosiveAnimate();

private:
	std::vector<GemCellEx*> m_vBoardData;
	CCSprite* m_pBoardBackground;

	BoardLayerDelegate* m_pDelegate;

	GemCellEx* m_pSelectedGem;

	int m_nUserIdx1;
	int m_nUserIdx2;

	bool m_bUserDrag;

	float m_fComboDeltaTime;
	int m_nCombo;

	bool m_bBoardImmobile;

	int m_nGemTypeNum;

	int m_canRemoveGamIdx1;
	int m_canRemoveGamIdx2;
	bool m_IsWhowHintCouldCreate;
	float m_UserHaveNotRemoveGemTime;
	bool m_IsUserMoveGem;

	bool m_IsRemoveGemNow;

	// 是否为矿石模式 
	bool m_isMineral;
	// 是否为第一次设置棋盘
	bool m_isFirstSetBoard;
	float m_MineralUpdateTime;
	// 矿石正在向上升起的时候，用户操作无效
	bool m_isAddMineralNow;

private:
	DISALLOW_COPY_AND_ASSIGN(BoardLayer);
};

#endif // _UI_LAYER_BOARD_LAYER_H_