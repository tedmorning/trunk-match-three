#ifndef _DATAPOOL_ELEMENT_TYPE_H_
#define _DATAPOOL_ELEMENT_TYPE_H_

enum ELE_TYPE {
	ELEMENT_UNKNOW,
	ELEMENT_FIRE,	        // 火
	ELEMENT_ICE,			// 冰
	ELEMENT_THUNDER,		// 雷
	ELEMENT_POISON,			// 毒
	ELEMENT_WATER,			// 水
	ELEMENT_LAND,			// 地
	ELEMENT_DARK,			// 暗
	// 矿石模式用到 
	ELEMENT_SOIL,            // 土壤 
	ELEMENT_STONR,           // 土壤加石头 
	ELEMENU_GOLD,            // 黄金矿石 

	// 改在上面的石头和黄金
	ELEMENU_STONR_SECOND,
	ELEMENU_GOLD_SECOND,
};

#define ELEMENT_COUNT 7

#endif
