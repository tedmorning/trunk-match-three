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
	/* 从xml节点中获取文本
	 * @param parentEle 当前节点
	 * @return          文本的内容,出错是为空
	 */
	std::string getCurElementText(const TiXmlElement *curEle);
	
	/* 从xml节点中获取int
	 * @param parentEle 当前节点
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          int型数据
	 */
	int getCurElementInt(const TiXmlElement *curEle, int nErrorRet = 0);

	/* 从xml节点中获取64位整型
	 * @param parentEle 父亲节点
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          64位整型
	 */
	long long getCurElementInt64(const TiXmlElement *curEle, long long nErrorRet = 0);

	/* 从xml节点中获取float
	 * @param parentEle 父亲节点
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          float型数据
	 */
	float getCurElementFloat(const TiXmlElement *curEle, float fErrorRet = 0.0f);

	/* 从xml节点中获取文本
	 * @param parentEle 父亲节点
	 * @param szTag     儿子节点的名称
	 * @return          文本的内容，出错是为空
	 */
	std::string getElementText(const TiXmlElement *parentEle, const char* szTag);

	/* 从xml节点中获取int数据
	 * @param parentEle 父亲节点
	 * @param szTag     儿子节点的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          int有符号类型数据
	 */
	int getElementInt(const TiXmlElement *parentEle, const char* szTag, int nErrorRet = 0);

	/* 从xml节点中获取64位整数数据
	 * @param parentEle 父亲节点
	 * @param szTag     儿子节点的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          64位有符号类型数据
	 */
	long long getElementInt64(const TiXmlElement *parentEle, const char* szTag, long long nErrorRet = 0);

	/* 从xml节点中获取float
	 * @param parentEle 父亲节点
	 * @param szTag     儿子节点的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          float
	 */
	float getElementFloat(const TiXmlElement *parentEle, const char* szTag, float nErrorRet = 0.0f);
	
	/* 从xml节点的属性中获取文本
	 * @param parentEle 当前节点
	 * @param szAttr    属性的名称
	 * @return          文本的内容,出错是为空
	 */
	std::string getCurAttributeText(const TiXmlElement *curEle, const char* szAttr);

	/* 从xml节点的属性中获取int数据
	 * @param parentEle 当前节点
	 * @param szAttr    属性的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          int有符号类型数据
	 */
	int getCurAttributeInt(const TiXmlElement *curEle, const char* szAttr, int nErrorRet = 0);

	/* 从xml节点的属性中获取int 64数据
	 * @param parentEle 当前节点
	 * @param szAttr    属性的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          64位有符号类型数据
	 */
	long long getCurAttributeInt64(const TiXmlElement *curEle, const char* szAttr, long long lErrorRet = 0);

	/* 从xml节点的属性中获取float数据
	 * @param parentEle 当前节点
	 * @param szAttr    属性的名称
	 * @param nErrorRet  解析错误时出现的返回值
	 * @return          64位有符号类型数据
	 */
	float getCurAttributeFloat(const TiXmlElement *curEle, const char* szAttr, float fErrorRet = 0.0f);
}

#endif