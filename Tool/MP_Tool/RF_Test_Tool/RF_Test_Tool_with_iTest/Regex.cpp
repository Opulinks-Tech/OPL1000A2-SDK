// Regex.cpp: implementation of the CRegex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TestRegex.h"
#include "Regex.h"
#pragma   comment(lib,   "Regex.lib ")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRegex::CRegex()
{
    	
}

CRegex::~CRegex()
{

}

bool CRegex::fnSearch( const char *pszSource,const char* pszExpre,std::vector<std::string>& vecResult,std::string& strError )
{
	
		CListResult  *listResult = 0;
		CListResult  *listError = 0;
		int iCount = ::fnSearch(pszSource,pszExpre,&listResult,&listError);
		vecResult.clear();
		while (0 != listResult)
		{
			string strResult = listResult->fnGetData();
			vecResult.push_back(strResult);
			listResult = listResult->pNext;
		}
		if (0 != listError)
		{
			strError = listError->fnGetData();
			return false;
		}
		fnDeleteResult(listResult);
		fnDeleteResult(listError);
		return true;
	
}

bool CRegex::fnGrep( const char *pszSource,const char* pszExpre,std::vector<std::string>& vecResult,std::string& strError )
{

		CListResult  *listResult = 0;
		CListResult  *listError = 0;
		::fnGrep(pszSource,pszExpre,&listResult,&listError);
	
		vecResult.clear();
		while (0 != listResult)
		{
			string strResult = listResult->fnGetData();
			vecResult.push_back(strResult);
			listResult = listResult->pNext;
		}
		if (0 != listError)
		{
			strError = listError->fnGetData();
		}
		
		fnDeleteResult(listResult);
		fnDeleteResult(listError);
		return true;
	
}

bool CRegex::fnRegReplace( const char *pszSource,const char* pszExpre,const char* pszNewString,std::string &strResult,std::string& strError )
{
	
		CListResult  *listResult = 0;
		CListResult  *listError = 0;
		::fnRegReplace(pszSource,pszExpre,pszNewString,&listResult,&listError);
		if (0 != listResult)
		{
			strResult = listResult->fnGetData();
		}
		if (0 != listError)
		{
			strError = listError->fnGetData();
		}
		fnDeleteResult(listResult);
		fnDeleteResult(listError);
		return true;
	
}
