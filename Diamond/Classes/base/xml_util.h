#ifndef XML_UTIL_H_
#define XML_UTIL_H_

#include <string>
#include "tinyxml.h"

namespace util
{

#define XML_BAD_RETURN -95533
#define XML_SET_ATTR_INT(_node, _from, _to) \
	{\
	int tInt = xml_util::getCurAttributeInt(_node, _from, XML_BAD_RETURN);\
	if(tInt != XML_BAD_RETURN) _to = tInt;\
	else _to = 0;							\
}
#define XML_SET_ATTR_FLOAT(_node, _from, _to) \
	{\
	f32 tInt = xml_util::getCurAttributeFloat(_node, _from, XML_BAD_RETURN);\
	if(tInt != XML_BAD_RETURN) _to = tInt;\
	else _to = 0.f;						\
}
#define XML_SET_ATTR_TEXT(_node, _from, _to) \
	{\
	std::string tStr = xml_util::getCurAttributeText(_node, _from);\
	if(tStr != "") _to = tStr;\
}

}

namespace xml_util
{
	/* ��xml�ڵ��л�ȡ�ı�
	 * @param parentEle ��ǰ�ڵ�
	 * @return          �ı�������,������Ϊ��
	 */
	std::string getCurElementText(const TiXmlElement *curEle);
	
	/* ��xml�ڵ��л�ȡint
	 * @param parentEle ��ǰ�ڵ�
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          int������
	 */
	int getCurElementInt(const TiXmlElement *curEle, int nErrorRet = 0);

	/* ��xml�ڵ��л�ȡ64λ����
	 * @param parentEle ���׽ڵ�
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          64λ����
	 */
	long long getCurElementInt64(const TiXmlElement *curEle, long long nErrorRet = 0);

	/* ��xml�ڵ��л�ȡfloat
	 * @param parentEle ���׽ڵ�
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          float������
	 */
	float getCurElementFloat(const TiXmlElement *curEle, float fErrorRet = 0.0f);

	/* ��xml�ڵ��л�ȡ�ı�
	 * @param parentEle ���׽ڵ�
	 * @param szTag     ���ӽڵ������
	 * @return          �ı������ݣ�������Ϊ��
	 */
	std::string getElementText(const TiXmlElement *parentEle, const char* szTag);

	/* ��xml�ڵ��л�ȡint����
	 * @param parentEle ���׽ڵ�
	 * @param szTag     ���ӽڵ������
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          int�з�����������
	 */
	int getElementInt(const TiXmlElement *parentEle, const char* szTag, int nErrorRet = 0);

	/* ��xml�ڵ��л�ȡ64λ��������
	 * @param parentEle ���׽ڵ�
	 * @param szTag     ���ӽڵ������
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          64λ�з�����������
	 */
	long long getElementInt64(const TiXmlElement *parentEle, const char* szTag, long long nErrorRet = 0);

	/* ��xml�ڵ��л�ȡfloat
	 * @param parentEle ���׽ڵ�
	 * @param szTag     ���ӽڵ������
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          float
	 */
	float getElementFloat(const TiXmlElement *parentEle, const char* szTag, float nErrorRet = 0.0f);
	
	/* ��xml�ڵ�������л�ȡ�ı�
	 * @param parentEle ��ǰ�ڵ�
	 * @param szAttr    ���Ե�����
	 * @return          �ı�������,������Ϊ��
	 */
	std::string getCurAttributeText(const TiXmlElement *curEle, const char* szAttr);

	/* ��xml�ڵ�������л�ȡint����
	 * @param parentEle ��ǰ�ڵ�
	 * @param szAttr    ���Ե�����
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          int�з�����������
	 */
	int getCurAttributeInt(const TiXmlElement *curEle, const char* szAttr, int nErrorRet = 0);

	/* ��xml�ڵ�������л�ȡint 64����
	 * @param parentEle ��ǰ�ڵ�
	 * @param szAttr    ���Ե�����
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          64λ�з�����������
	 */
	long long getCurAttributeInt64(const TiXmlElement *curEle, const char* szAttr, long long lErrorRet = 0);

	/* ��xml�ڵ�������л�ȡfloat����
	 * @param parentEle ��ǰ�ڵ�
	 * @param szAttr    ���Ե�����
	 * @param nErrorRet  ��������ʱ���ֵķ���ֵ
	 * @return          64λ�з�����������
	 */
	float getCurAttributeFloat(const TiXmlElement *curEle, const char* szAttr, float fErrorRet = 0.0f);
}

#endif