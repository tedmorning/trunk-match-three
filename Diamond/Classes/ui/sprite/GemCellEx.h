#ifndef _UI_SPRITE_GEM_CELL_EX_H_
#define _UI_SPRITE_GEM_CELL_EX_H_

#include "cocos2d.h"
#include "Button.h"
#include "ElementType.h"

USING_NS_CC;

enum DragDirection
{
	DIRECTION_UNKNOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
};

enum GemCellModifier
{
	GCM_NORMAL,			 // ��ͨ״̬
	GCM_EXPLOSIVE,       // ��ը״̬
	GCM_ROWCOLUMNREMOVE, // ��������
	GCM_HYPERCUBE        // ը��������������ĳһ����ɫ�����ӣ�
};

enum GemCellRemoveType
{
	GCRT_NORMAL,					// ��ͨ�Ƴ�
	GCRT_EXPLOSIVE,					// ��ը�Ƴ�
	GCRT_ROWCOLUMNREMOVE,			// ��������
	GCRT_HYPERCUBE,					// ͬɫ����
	GCRT_AUTO,						// �Զ�����
	GCRT_EXPLOSIVE_COMPOSE,			// ��ը�ϳɱ�ը��������
	GCRT_ROWCOLUMNREMOVE_COMPOSE,	// �ϳ�������������
	GCRT_HYPERCUBE_COMPOSE			// �ϳ�ͬɫ��������
};

class GemCellEx;

class GemCellExDelegate : public ButtonDelegate
{
public:
	virtual void onTouchDrag(GemCellEx* gem, DragDirection dire) {}
	virtual void onGemSelected(GemCellEx* gem) {}
	virtual void onGemUnselected(GemCellEx* gem) {}
	virtual void onGemExplosiveModifier(GemCellEx* gem) {}
	virtual void onGemRowColumnRemoveModifier(GemCellEx* gem) {}
	virtual void onGemHypercubeModifier(GemCellEx* gem) {}

protected:
	virtual ~GemCellExDelegate() {}
};

class GemCellEx : public Button
{
public:
	GemCellEx(ELE_TYPE eleType);
	GemCellEx();
	~GemCellEx();
	

public:
	static GemCellEx* create(ELE_TYPE eleType, float size);

public:
	void setDelegate(GemCellExDelegate* delegate) { m_pGemDelegate = delegate; m_pDelegate = delegate; }

	void setElement(ELE_TYPE ele_type) { m_eGemCellType = ele_type; }
	ELE_TYPE element() const { return m_eGemCellType; }

	void setRow(int row) { m_nRow = row; }
	int row() const { return m_nRow; }

	void setColumn(int column) { m_nColumn = column; }
	int column() const { return m_nColumn; }

	bool shouldBeRemoved() const { return m_bShouldBeRemoved; }
	void setShouldBeRemoved(bool value) { m_bShouldBeRemoved = value; }

	GemCellModifier modifier() const { return m_eModifier; }
	void setModifier(GemCellModifier modifier) { m_eModifier = modifier; }

	GemCellRemoveType removeType() const { return m_eRemoveType; }
	void setRemoveType(GemCellRemoveType type) { m_eRemoveType = type; }

	bool invincible() const { return m_bInvincible; }
	void setInvincible(bool invincible) { m_bInvincible = invincible; }

	bool explodedOnce() const { return m_bExplodedOnce; }
	void setExplodedOnce(bool explode) { m_bExplodedOnce = explode; }

	void setTextureByEleType(ELE_TYPE eleType);

	void setSelected();
	void cancelSelected();

	void updateTex();

protected:
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

private:
	bool initWithEleType();
	std::string getTexureByEleType(ELE_TYPE eleType);

private:
	CCPoint m_ptTouchDown;
	GemCellExDelegate* m_pGemDelegate;

	// �������������������к���
	int m_nRow;
	int m_nColumn;

	// �Ƿ񼴽����Ƴ�
	bool m_bShouldBeRemoved;

	// �Ƴ���ʽ
	GemCellRemoveType m_eRemoveType;

	// ����������״̬
	GemCellModifier m_eModifier;

	// �����Ƿ��ȶ��ģ��Ƿ���Ƴ���
	bool m_bInvincible;

	// ��ʶ�Ƿ��Ѿ���ը
	bool m_bExplodedOnce;

	// ��������
	ELE_TYPE m_eGemCellType;

private:
	DISALLOW_COPY_AND_ASSIGN(GemCellEx);
};

#endif // _UI_SPRITE_GEM_CELL_EX_H_