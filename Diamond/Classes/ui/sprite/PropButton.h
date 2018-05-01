#ifndef _UI_SPRITE_PROP_BUTTON_H_
#define _UI_SPRITE_PROP_BUTTON_H_

#include "Button.h"

class PropButton : public Button
{
public:
	PropButton(int nPropId);
	~PropButton();

public:
	static PropButton* create(int nPropId, int nPropNum, const char* frame);

public:
	int getPropId() const { return m_nPropId; }
	void setPropNum(int num);

protected:
	bool init(int nPropNum, const char* frame);

private:
	int m_nPropId;
	CCLabelBMFont* m_pLabelPropNum;
	// CCLabelAtlas* m_pLabelPropNum;
private:
	DISALLOW_COPY_AND_ASSIGN(PropButton);
};

#endif // _UI_SPRITE_PROP_BUTTON_H_