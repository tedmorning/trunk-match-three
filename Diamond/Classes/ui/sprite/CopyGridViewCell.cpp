#include "CopyGridViewCell.h"
#include "CopyStar.h"
#include "DataPool.h"

const int kLockedSpriteTag = 0x0001;
const int kCopyStartTag = 0x0002;
const int kButtonTag = 0x0003;

CopyGridViewCell::CopyGridViewCell( CopyData* pCopyData)
: m_pCopyData(pCopyData)
, m_pDelegate(NULL)
{
}

CopyGridViewCell::CopyGridViewCell()
{
}

CopyGridViewCell::~CopyGridViewCell()
{
}

int CopyGridViewCell::getCopyId() const
{
	if (m_pCopyData) {
		return m_pCopyData->getId();
	}
	return 0;
}

CopyGridViewCell* CopyGridViewCell::create(CopyGridViewCellDelegate* delegate, CopyData* pCopyData, int idx)
{
	CopyGridViewCell* cell = new CopyGridViewCell(pCopyData);
	if (cell && cell->init(idx)) {
		cell->setDelegate(delegate);
		cell->autorelease();
		return cell;
	}
	CC_SAFE_DELETE(cell);
	return NULL;
}

bool CopyGridViewCell::init(int idx)
{
	if (!CCGridViewCell::init()) {
		return false;
	}

	if (m_pCopyData) {
		if (m_pCopyData->isLocked()) {
			CCSprite* pSpriteLocked = CCSprite::create("copy_locked.png");
			pSpriteLocked->setTag(kLockedSpriteTag);
			pSpriteLocked->setAnchorPoint(CCPointZero);
			pSpriteLocked->setPosition(CCPointZero);
			this->addChild(pSpriteLocked);
			pSpriteLocked->setScale(0.5f);
		}
		else {
			createEnableCopyItem(idx);
		}
	}
	return true;
}

void CopyGridViewCell::onTouchUp(Button* button)
{
	if (m_pDelegate) {
		m_pDelegate->onCopyItemClick(this, m_pCopyData, NULL);
	}
}

void CopyGridViewCell::createEnableCopyItem(int idx)
{
	Button* pSpriteBg = dynamic_cast<Button*>(getChildByTag(kButtonTag));
	if (pSpriteBg == NULL) {

		CCString* string = CCString::createWithFormat("copy_bg%d.png", idx);
		const char* pImage = string->getCString(); //  根据idx设置图片的资源名字 
		pSpriteBg = Button::createWithFile(pImage);
		// pSpriteBg = Button::createWithFile("copy_bg.png");
		pSpriteBg->setTag(kButtonTag);
		pSpriteBg->setAnchorPoint(CCPointZero);
		pSpriteBg->setPosition(CCPointZero);
		pSpriteBg->registerControlEvent();
		pSpriteBg->setDelegate(this);
		this->addChild(pSpriteBg);
	}

	if (getChildByTag(kCopyStartTag) == NULL) {
		pSpriteBg->setScale(0.5f);
		if (getConfigCopy().getConfigCopyItem(m_pCopyData->getId())->infinite)
		{
			return;
		}
		CopyStar* star = CopyStar::create(m_pCopyData->getScore());
		star->setTag(kCopyStartTag);
		star->setAnchorPoint(CCPointZero);
		star->setPosition(CCPointZero);
		pSpriteBg->addChild(star);
	}
}
