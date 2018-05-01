#ifndef STL_UTIL_H
#define STL_UTIL_H

#include "cocos2d.h"
#include <list>

using namespace cocos2d;

template<class TKey, class TValue> inline void clearMap(std::map<TKey, TValue* >& refMap)
{
    typename std::map<TKey, TValue* >::iterator iteBegin = refMap.begin();
    typename std::map<TKey, TValue* >::iterator iteEnd = refMap.end();

    while (iteBegin != iteEnd)
    {
        delete iteBegin->second;
        iteBegin++;
    }
    refMap.clear();
}

template<class T> inline void clearVector(std::vector<T* >& refVector)
{
    typename std::vector<T* >::iterator iteBegin = refVector.begin();
    typename std::vector<T* >::iterator iteEnd = refVector.end();

    while (iteBegin != iteEnd)
    {
        delete *iteBegin;
        iteBegin++;
    }
    refVector.clear();
}

template<class T> inline void clearList(std::list<T* >& refList)
{
    typename std::list<T* >::iterator iteBegin = refList.begin();
    typename std::list<T* >::iterator iteEnd = refList.end();

    while (iteBegin != iteEnd)
    {
        delete *iteBegin;
        iteBegin++;
    }
    refList.clear();
}

#endif // STL_UTIL_H