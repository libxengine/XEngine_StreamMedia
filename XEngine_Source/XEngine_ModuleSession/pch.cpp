#include "pch.h"
#include "ModuleSession_PullStream/ModuleSession_PullStream.h"
#include "ModuleSession_PushStream/ModuleSession_PushStream.h"
/********************************************************************
//    Created:     2022/04/25  10:21:36
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Session_IsErrorOccur = false;
XLONG Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleSession_PullStream m_PullStream;
CModuleSession_PushStream m_PushStream;
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/*********************************************************************************
*                          拉流端导出会话模块                                    *
*********************************************************************************/
extern "C" bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, LPCXSTR lpszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
	return m_PullStream.ModuleSession_PullStream_Insert(lpszClientAddr, lpszSMSAddr, lpszPushAddr, enStreamType);
}
extern "C" bool ModuleSession_PullStream_GetSMSAddr(LPCXSTR lpszClientAddr, XCHAR * ptszSMSAddr)
{
	return m_PullStream.ModuleSession_PullStream_GetSMSAddr(lpszClientAddr, ptszSMSAddr);
}
extern "C" bool ModuleSession_PullStream_GetPushAddr(LPCXSTR lpszClientAddr, XCHAR * ptszPushAddr)
{
	return m_PullStream.ModuleSession_PullStream_GetPushAddr(lpszClientAddr, ptszPushAddr);
}
extern "C" bool ModuleSession_PullStream_GetStreamType(LPCXSTR lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE * penStreamType)
{
	return m_PullStream.ModuleSession_PullStream_GetStreamType(lpszClientAddr, penStreamType);
}
extern "C" bool ModuleSession_PullStream_RTCSet(LPCXSTR lpszClientAddr, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass, LPCXSTR lpszHMacStr)
{
	return m_PullStream.ModuleSession_PullStream_RTCSet(lpszClientAddr, lpszICEUser, lpszICEPass, lpszHMacStr);
}
extern "C" bool ModuleSession_PullStream_RTCGet(LPCXSTR lpszClientAddr, XCHAR * ptszICEUser, XCHAR * ptszICEPass, XCHAR * ptszHMacStr)
{
	return m_PullStream.ModuleSession_PullStream_RTCGet(lpszClientAddr, ptszICEUser, ptszICEPass, ptszHMacStr);
}
extern "C" bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr)
{
	return m_PullStream.ModuleSession_PullStream_Delete(lpszClientAddr);
}
extern "C" bool ModuleSession_PullStream_PublishDelete(LPCXSTR lpszClientAddr)
{
	return m_PullStream.ModuleSession_PullStream_PublishDelete(lpszClientAddr);
}
extern "C" bool ModuleSession_PullStream_GetList(STREAMMEDIA_PULLLISTINFO * **pppSt_PullList, int* pInt_ListCount)
{
	return m_PullStream.ModuleSession_PullStream_GetList(pppSt_PullList, pInt_ListCount);
}
extern "C" bool ModuleSession_PullStream_FLVTagSet(LPCXSTR lpszClientAddr, int nTagSize)
{
	return m_PullStream.ModuleSession_PullStream_FLVTagSet(lpszClientAddr, nTagSize);
}
extern "C" bool ModuleSession_PullStream_FLVTagGet(LPCXSTR lpszClientAddr, int* pInt_TagSize)
{
	return m_PullStream.ModuleSession_PullStream_FLVTagGet(lpszClientAddr, pInt_TagSize);
}
/*********************************************************************************
*                          推流导出会话模块                                      *
*********************************************************************************/
extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
	return m_PushStream.ModuleSession_PushStream_Create(lpszClientAddr, lpszSMSAddr, enStreamType);
}
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_Destroy(lpszClientAddr);
}
extern "C" bool ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR * ptszSMSAddr)
{
	return m_PushStream.ModuleSession_PushStream_GetAddrForAddr(lpszClientAddr, ptszSMSAddr);
}
extern "C" bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR * ptszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_FindStream(lpszSMSAddr, ptszClientAddr);
}
extern "C" bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
	return m_PushStream.ModuleSession_PushStream_ClientInsert(lpszClientAddr, lpszPullAddr, enStreamType);
}
extern "C" bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr)
{
	return m_PushStream.ModuleSession_PushStream_ClientDelete(lpszClientAddr, lpszPullAddr);
}
extern "C" bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<STREAMMEDIA_SESSIONCLIENT>*pStl_ListClient)
{
	return m_PushStream.ModuleSession_PushStream_ClientList(lpszClientAddr, pStl_ListClient);
}
extern "C" bool ModuleSession_PushStream_SetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO * pSt_AVInfo)
{
	return m_PushStream.ModuleSession_PushStream_SetAVInfo(lpszClientAddr, pSt_AVInfo);
}
extern "C" bool ModuleSession_PushStream_GetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO * pSt_AVInfo)
{
	return m_PushStream.ModuleSession_PushStream_GetAVInfo(lpszClientAddr, pSt_AVInfo);
}
extern "C" bool ModuleSession_PushStream_GetInfo(STREAMMEDIA_PUBLISHINFO * **pppSt_ProtocolStream, int* pInt_ListCount)
{
	return m_PushStream.ModuleSession_PushStream_GetInfo(pppSt_ProtocolStream, pInt_ListCount);
}
extern "C" bool ModuleSession_PushStream_HLSInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszTSFile, XNETHANDLE xhToken)
{
	return m_PushStream.ModuleSession_PushStream_HLSInsert(lpszClientAddr, lpszTSFile, xhToken);
}
extern "C" bool ModuleSession_PushStream_HLSGetFile(LPCXSTR lpszClientAddr, XCHAR * ptszFileName)
{
	return m_PushStream.ModuleSession_PushStream_HLSGetFile(lpszClientAddr, ptszFileName);
}
extern "C" bool ModuleSession_PushStream_HLSWrite(LPCXSTR lpszClientAddr, LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	return m_PushStream.ModuleSession_PushStream_HLSWrite(lpszClientAddr, lpszMSGBuffer, nMSGLen);
}
extern "C" bool ModuleSession_PushStream_HLSClose(LPCXSTR lpszClientAddr, XNETHANDLE * pxhToken)
{
	return m_PushStream.ModuleSession_PushStream_HLSClose(lpszClientAddr, pxhToken);
}
extern "C" bool ModuleSession_PushStream_HLSTimeSet(LPCXSTR lpszClientAddr, __int64u nTime)
{
	return m_PushStream.ModuleSession_PushStream_HLSTimeSet(lpszClientAddr, nTime);
}
extern "C" bool ModuleSession_PushStream_HLSTimeGet(LPCXSTR lpszClientAddr, __int64u * pInt_Time)
{
	return m_PushStream.ModuleSession_PushStream_HLSTimeGet(lpszClientAddr, pInt_Time);
}