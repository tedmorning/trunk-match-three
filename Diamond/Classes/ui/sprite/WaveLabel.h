#ifndef _UI_SPRITE_WAVE_LABEL_H_
#define _UI_SPRITE_WAVE_LABEL_H_

#include "cocos2d.h"

USING_NS_CC;

class WaveLabel : public CCLabelBMFont
{
public:
	WaveLabel();
	~WaveLabel();

public:
	static WaveLabel* create(int nTotalWave, bool infiniti = false);

	void setWave(int cur, int total);
	void setInfinitiTrue(bool bo){m_isInfiniti = bo;}

private:
	DISALLOW_COPY_AND_ASSIGN(WaveLabel);
	bool m_isInfiniti;
};

#endif // _UI_SPRITE_WAVE_LABEL_H_