#ifndef _UI_LAYER_BOARD_LAYER_H_
#define _UI_LAYER_BOARD_LAYER_H_

#include <map>
#include "cocos2d.h"
#include "GemCellEx.h"
/*#include "BaseLayer.h"*/
USING_NS_CC;
// combo���ճ���������˼

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
	// ��������
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
	virtual void onTouchDown(Button* button);                      // ����
	virtual void onTouchDrag(GemCellEx* gem, DragDirection dire);  // ����
	virtual void onGemSelected(GemCellEx* gem);             // ѡ������
	virtual void onGemUnselected(GemCellEx* gem);           // ȡ��ѡ������
	virtual void onGemExplosiveModifier(GemCellEx* gem);    // ��ը����
	virtual void onGemRowColumnRemoveModifier(GemCellEx* gem);  // ��������
	virtual void onGemHypercubeModifier(GemCellEx* gem);        // ͬɫ����
	 
private:
	////////////////////////////////////////////////////////////////////////////
	// Action
	void gemMoveAction(GemCellEx* gem, const CCPoint& pt, float time = 0.5f, CCCallFuncND* callback = NULL);
	void gemMoveAndReverseAction(GemCellEx* gem, const CCPoint& pt);

	void shuffleDown();
	void shuffleAllDown();
	void fillBoard();
	void shuffleDown(const std::vector<GemCellEx*>& vGem, float time = 0.5f);

	void exchangeGemAndReverseAction(GemCellEx* gem1, GemCellEx* gem2); // ���ܻ��������ӵĻظ�ԭλ����
	void exchangeGemAction(GemCellEx* gem1, GemCellEx* gem2); 
	void exchangeAction(GemCellEx* gem1, GemCellEx* gem2);   // �������ӵĶ���

	void hideHint();

	// NOTE: 
	// 1��ʮ��������ը��ͬɫ���������ӱ���������ը�Ķ�����0.9�룬
	// 2���������Ž����󣬽��������жϣ�����Ǳ�ը������������������������һ��

	// ʮ��������ը��ͬɫ���������ӱ���������ը�Ķ���

	float beforeExplosiveAction(); // ��ը֮ǰ����Ч

	// NOTE:
	// COMBO�����㷽ʽ��
	// ����ʱ����û�л������ӣ���combo���¿�ʼ
	// �����interval��ʱ����в�������������£���Ϊ�����Զ�����������combo��������ȥ
	// float m_fComboDeltaTime;
	// int m_nCombo;
	// virtual void update(float dt) { if (m_fComboDeltaTime > value) { m_nCombo = 0; } else { m_fComboDeltaTime += dt; } }

	////////////////////////////////////////////////////////////////////////////
	// Action Effect
	// NOTE: �����ʱ����д�������л���������װ
	CCActionInterval* createExplosiveAction();
	CCActionInterval* createRowColumnRemoveAction(LightingType direction);
	CCActionInterval* createHypercubeAction();

	////////////////////////////////////////////////////////////////////////////
	// callback
	void cbGemMoveAction(CCNode* node, void* data);				// �����ƶ�����
	void cbRemoveGemAction(CCNode* node, void* data);			// ɾ�����Ӷ���
	void cbRemoveGemActionForMineral(CCNode* node, void* data);			// ɾ�����Ӷ���
	void cbChangeGemModifierAction(CCNode* node, void* data);	// 
	void cbShuffleDownAction(CCNode* node, void* data);
	void cbFillBoardAction(CCNode* node, void* data);
	void cbMarkComboAction(CCNode* node, void* data);
	void cbExplosiveActionEnd(CCNode* node, void* data);
	void cbBeginExplosiveAction(CCNode* node, void* data);
	void cbGemMoveActionCallback(CCNode* node, void* data);
	void goldMineralExplosiveCallback(CCNode* node, void* data);// ʯͷ��ը��callback
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
	// ����һ�п�ʯ 
	void boardAddMineral();
	// �����ʯģʽ����������
	void addMinderalGem(int row, int column, std::vector<GemCellEx*>* vMineral, bool isNeedTwice = true, bool isRowColumn = false);

	void selectedGem(GemCellEx* gem);
	void deselectCurrentGem();

	bool cellInBoard(int row, int column);

	GemCellEx* createMineralBlock(int row, int column, int starRow, bool isInsertVector = false); // ����һ�ſ�ʯ����
	GemCellEx* createBlock(int row, int column, int startRow); // ����һ������ 
	GemCellEx* safeGetCell(int row, int column); 
	GemCellEx* safeGetCell(unsigned int index); 

	ELE_TYPE safeGetCellType(int row, int column);
	CCPoint getGemPosition(int row, int column);

	int index(int row, int column) const;
	// �������������е�һ������
	GemCellEx* board(int row, int column);
	void setCell(int row, int column, GemCellEx* value, bool isInsertVector = false);
	void exchangeGemCellPos(GemCellEx* gem1, GemCellEx* gem2);

	ELE_TYPE generateGemCellType();
	// �������һ�ֿ�ʯģʽ������
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

	// �Ƿ���Ҫ�������������
	bool isNeedShuffleDown();
	bool isNeedFillBoard();

	void resetInvincibleStatus();
	void resetComboDeltaTime();
	void addCombo();

	void swapGem(GemCellEx* gem1, GemCellEx* gem2); // �����������ӵ�λ��
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

	// �Ƿ�Ϊ��ʯģʽ 
	bool m_isMineral;
	// �Ƿ�Ϊ��һ����������
	bool m_isFirstSetBoard;
	float m_MineralUpdateTime;
	// ��ʯ�������������ʱ���û�������Ч
	bool m_isAddMineralNow;

private:
	DISALLOW_COPY_AND_ASSIGN(BoardLayer);
};

#endif // _UI_LAYER_BOARD_LAYER_H_