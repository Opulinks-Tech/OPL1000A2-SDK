// Regex.h: interface for the CRegex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGEX_H__1AA17863_7891_46A5_BD05_BA387C1D1A14__INCLUDED_)
#define AFX_REGEX_H__1AA17863_7891_46A5_BD05_BA387C1D1A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(disable: 4786)
#include "IRegex.h"
class CRegex  
{
public:
	CRegex();
	virtual ~CRegex();
	//////////////////////////////////////////////////////////////////////////
    //函数说明:
    //字符串查找,如pszSource=abc123cde pszExpre=(\d*).* 则结果为[0]123cde [1]123
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //vecResult[OUT]:查找得到的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //查找成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual bool fnSearch(const char *pszSource,const char* pszExpre,std::vector<std::string>& vecResult,std::string& strError);
    //////////////////////////////////////////////////////////////////////////
    //函数说明:
    //字符串匹配,如pszSource=abc123cde321 pszExpre=\d* 则结果为[0]123  [1]321
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //vecResult[OUT]:匹配得到的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //匹配成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual bool fnGrep(const char *pszSource,const char* pszExpre,std::vector<std::string>& vecResult,std::string& strError);
    //////////////////////////////////////////////////////////////////////////
    //函数说明:
    //正则式替换,找出与正则式一致的字符串并替换
    //参数说明:
    //pszSource[IN]:源字符串
    //pszExpre[IN]:正则表达式
    //pszNewString[IN]:被替换的新字符串
    //strResult[OUT]:替换后的结果
    //strError[OUT]:失败时返回的错误信息
    //返回说明:
    //匹配成功返回true,查找失败返回false
    //////////////////////////////////////////////////////////////////////////
    virtual bool fnRegReplace(const char *pszSource,const char* pszExpre,const char* pszNewString,std::string &strResult,std::string& strError);
	
private:


};

#endif // !defined(AFX_REGEX_H__1AA17863_7891_46A5_BD05_BA387C1D1A14__INCLUDED_)
