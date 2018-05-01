#include "ActionNumTo.h"

ActionNumTo::ActionNumTo()
: m_fDeltaNum(0.f)
, m_uStart(0)
, m_uEnd(0)
, m_uTotal(0)
{
}

ActionNumTo::~ActionNumTo()
{
}

ActionNumTo* ActionNumTo::create(float duration, GLuint start, GLuint end)
{
    ActionNumTo* pNumTo = new ActionNumTo();
    pNumTo->initWithDuration(duration, start, end);
    pNumTo->autorelease();

    return pNumTo;
}

ActionNumTo* ActionNumTo::create(float duration, GLuint start, GLuint end, GLuint total)
{
	ActionNumTo* pNumTo = new ActionNumTo();
	pNumTo->initWithDuration(duration, start, end, total);
	pNumTo->autorelease();

	return pNumTo;
}

bool ActionNumTo::initWithDuration(float duration, GLuint start, GLuint end, GLuint total)
{
    if (duration == 0.f)
    {
        duration = 0.001f;
    }

    if (CCActionInterval::initWithDuration(duration))
    {
        m_uStart = start;
        m_uEnd = end;
		m_uTotal = total;
        return true;
    }

    return false;
}

CCObject* ActionNumTo::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    ActionNumTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        pCopy = (ActionNumTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new ActionNumTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_uStart, m_uEnd);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void ActionNumTo::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    m_fDeltaNum = (int)m_uEnd - (int)m_uStart;
}

void ActionNumTo::update(float time)
{
    CCLabelBMFont* pTarget = dynamic_cast<CCLabelBMFont*>(m_pTarget);
    if (pTarget)
    {
        int num = (int)m_uStart + m_fDeltaNum * (m_elapsed / m_fDuration);
        if ((m_fDeltaNum > 0) && (num > m_uEnd))
        {
            num = m_uEnd;
        }

        if ((m_fDeltaNum < 0) && (num < m_uEnd))
        {
            num = m_uEnd;
        }

		if (m_elapsed >= m_fDuration)
		{
			num = m_uEnd;
		}

		if (m_uTotal > 0)
		{
			pTarget->setString(CCString::createWithFormat("%d/%d", num, m_uTotal)->getCString());
		}
		else
		{
			pTarget->setString(CCString::createWithFormat("%d", num)->getCString());
		}
    }
}
