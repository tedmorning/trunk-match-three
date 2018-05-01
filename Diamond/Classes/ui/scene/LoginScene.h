#ifndef _UI_SCENE_LOGIN_SCENE_H_
#define _UI_SCENE_LOGIN_SCENE_H_

#include "cocos2d.h"
#include "BoardLayer.h"

class LoginScene : public cocos2d::CCLayer, public BoardLayerDelegate
{
public:
    virtual bool init();  

    static cocos2d::CCScene* scene();

    CREATE_FUNC(LoginScene);

	virtual void onCombo(int combo);
	virtual void onGemExplode(std::map<GemCellRemoveType, std::vector<GemCellEx*> > mExplodeGem);
};

#endif  // _UI_SCENE_MAIN_SCENE_H_