#ifndef _DATAPOOL_ELEMENT_TYPE_H_
#define _DATAPOOL_ELEMENT_TYPE_H_

enum ELE_TYPE {
	ELEMENT_UNKNOW,
	ELEMENT_FIRE,	        // ��
	ELEMENT_ICE,			// ��
	ELEMENT_THUNDER,		// ��
	ELEMENT_POISON,			// ��
	ELEMENT_WATER,			// ˮ
	ELEMENT_LAND,			// ��
	ELEMENT_DARK,			// ��
	// ��ʯģʽ�õ� 
	ELEMENT_SOIL,            // ���� 
	ELEMENT_STONR,           // ������ʯͷ 
	ELEMENU_GOLD,            // �ƽ��ʯ 

	// ���������ʯͷ�ͻƽ�
	ELEMENU_STONR_SECOND,
	ELEMENU_GOLD_SECOND,
};

#define ELEMENT_COUNT 7

#endif
