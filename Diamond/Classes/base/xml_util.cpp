#include "xml_util.h"
#include "cocos2d.h"

static const int INT_32_TYPE = 1;
static const int UNSIGNED_INT_32_TYPE = 2;
static const int INT_64_TYPE = 3;
static const int UNSIGNED_INT_64_TYPE = 4;
static const int FLOAT_TYPE = 5;

namespace xml_util
{
	bool getElementNumber(const TiXmlElement* curEle, int nNumType, void* pOutNumber)
	{
		if(curEle && pOutNumber)
		{
			const char* szValue = curEle->GetText();
			if(szValue)
			{
				if(INT_32_TYPE == nNumType)
				{
					sscanf(szValue, "%d", (int*)pOutNumber);
				}
				else if(UNSIGNED_INT_32_TYPE == nNumType)
				{
					sscanf(szValue, "%u", (unsigned int*)pOutNumber);
				}
				else if(INT_64_TYPE == nNumType)
				{
					sscanf(szValue, "%lld", (long long*)pOutNumber);
				}
				else if(UNSIGNED_INT_64_TYPE == nNumType)
				{
					sscanf(szValue, "%llu", (unsigned long long*)pOutNumber);
				}
				else
				{
					sscanf(szValue, "%f", (float*)pOutNumber);
				}
				return true;
			}
		}
		return false;
	}

	bool getElementNumber(const TiXmlElement *parentEle, const char* szTag,  int nNumType, void* pOutNumber)
	{
		if(parentEle && szTag && pOutNumber)
		{
			const TiXmlElement *childEle = parentEle->FirstChildElement(szTag);
			if(childEle)
			{
				const char* szValue = childEle->GetText();

				if(szValue)
				{
					if(INT_32_TYPE == nNumType)
					{
						sscanf(szValue, "%d", (int*)pOutNumber);
					}
					else if(UNSIGNED_INT_32_TYPE == nNumType)
					{
						sscanf(szValue, "%u", (unsigned int*)pOutNumber);
					}
					else if(INT_64_TYPE == nNumType)
					{
						sscanf(szValue, "%lld", (long long*)pOutNumber);
					}
					else if(UNSIGNED_INT_64_TYPE == nNumType)
					{
						sscanf(szValue, "%llu", (unsigned long long*)pOutNumber);
					}
					else
					{
						sscanf(szValue, "%f", (float*)pOutNumber);
					}
					return true;
				}
			}
			return false;
		}
		return false;
	}

	bool getAttributeNumber(const TiXmlElement *curEle, const char* szAttr, int nNumType, void* pOutNumber)
	{
		if(curEle && szAttr && pOutNumber)
		{
			const char* szValue = curEle->Attribute(szAttr);
			if(szValue)
			{
				if(INT_32_TYPE == nNumType)
				{
					sscanf(szValue, "%d", (int*)pOutNumber);
				}
				else if(UNSIGNED_INT_32_TYPE == nNumType)
				{
					sscanf(szValue, "%u", (unsigned int*)pOutNumber);
				}
				else if(INT_64_TYPE == nNumType)
				{
					sscanf(szValue, "%lld", (long long*)pOutNumber);
				}
				else if(UNSIGNED_INT_64_TYPE == nNumType)
				{
					sscanf(szValue, "%llu", (unsigned long long*)pOutNumber);
				}
				else 
				{
					sscanf(szValue, "%f", (float*)pOutNumber);
				}
				return true;
			}
			return false;
		}
		return false;
	}

	std::string getCurElementText(const TiXmlElement *curEle)
	{
		if(curEle)
			return curEle->GetText();
		return 0;

	}

	int getCurElementInt(const TiXmlElement *curEle, int nErrorRet)
	{
		int iIntNum = 0;
		if(getElementNumber(curEle, INT_32_TYPE, &iIntNum))
		{
			return iIntNum;
		}
		return nErrorRet;
	}

	long long getCurElementInt64(const TiXmlElement *curEle, long long nErrorRet)
	{
		long long lInt64Num = 0;
		if(getElementNumber(curEle, INT_64_TYPE, &lInt64Num))
		{
			return lInt64Num;
		}
		return lInt64Num;
	}

	float getCurElementFloat(const TiXmlElement *curEle, float fErrorRet)
	{
		float fNum = 0;
		if(getElementNumber(curEle, FLOAT_TYPE, &fNum))
		{
			return fNum;
		}
		return fErrorRet;
	}

	std::string getElementText(const TiXmlElement *parentEle, const char* szTag)
	{
		if(parentEle && szTag)
		{
			const TiXmlElement *childEle = parentEle->FirstChildElement(szTag);
			if(childEle)
			{
				return childEle->GetText();
			}
		}
		return "";
	}

	int getElementInt(const TiXmlElement *parentEle, const char* szTag, int nErrorRet)
	{
		int iIntNum = 0;
		if(getElementNumber(parentEle, szTag, INT_32_TYPE, &iIntNum))
		{
			return iIntNum;
		}
		return nErrorRet;
	}

	long long getElementInt64(const TiXmlElement *parentEle, const char* szTag, long long lErrorRet)
	{
		long long lInt64Num = 0;
		if(getElementNumber(parentEle, szTag, INT_64_TYPE, &lInt64Num))
		{
			return lInt64Num;
		}
		return lErrorRet;
	}

	float getElementFloat(const TiXmlElement *parentEle, const char* szTag, float nErrorRet)
	{
		float fNum = 0;
		if(getElementNumber(parentEle, szTag, FLOAT_TYPE, &fNum))
		{
			return fNum;
		}
		return nErrorRet;
	}

	std::string getCurAttributeText(const TiXmlElement *curEle, const char* szAttr)
	{
		if(curEle && szAttr)
		{
			const char* szValue = curEle->Attribute(szAttr);
			if (szValue)
			{
				return szValue;
			}
		}
		return "";
	}

	int getCurAttributeInt(const TiXmlElement *curEle, const char* szAttr, int nErrorRet)
	{
		int iIntNum = 0;
		if( getAttributeNumber(curEle, szAttr, INT_32_TYPE, &iIntNum) )
		{
			return iIntNum;
		}
		return nErrorRet;
	}

	long long getCurAttributeInt64(const TiXmlElement *curEle, const char* szAttr, long long lErrorRet)
	{
		long long lInt64Num = 0;
		if(getAttributeNumber(curEle, szAttr, INT_64_TYPE, &lInt64Num))
		{
			return lInt64Num;
		}
		return lErrorRet;
	}

	float getCurAttributeFloat(const TiXmlElement *curEle, const char* szAttr, float fErrorRet)
	{
		float fNum = 0;
		if(getAttributeNumber(curEle, szAttr, FLOAT_TYPE, &fNum))
		{
			return fNum;
		}
		return fErrorRet;
	}
}