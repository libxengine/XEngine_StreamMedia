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
extern "C" bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr)
{
	return m_PullStream.ModuleSession_PullStream_Delete(lpszClientAddr);
}
/*********************************************************************************
*                          推流导出会话模块                                      *
*********************************************************************************/
extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr)
{
	return m_PushStream.ModuleSession_PushStream_Create(lpszClientAddr, lpszSMSAddr);
}
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_Destroy(lpszClientAddr);
}
extern "C" bool ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR * ptszSMSAddr)
{
	return m_PushStream.ModuleSession_PushStream_GetAddrForAddr(lpszClientAddr, ptszSMSAddr);
}
extern "C" bool ModuleSession_PushStream_SetHDRBuffer(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	return m_PushStream.ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
extern "C" bool ModuleSession_PushStream_GetHDRBuffer(LPCXSTR lpszClientAddr, XCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_PushStream.ModuleSession_PushStream_GetHDRBuffer(lpszClientAddr, ptszMsgBuffer, pInt_MsgLen);
}
extern "C" bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR * ptszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_FindStream(lpszSMSAddr, ptszClientAddr);
}
extern "C" bool ModuleSession_PushStream_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType)
{
	return m_PushStream.ModuleSession_PushStream_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen, nAVType, nFrameType);
}
extern "C" bool ModuleSession_PushStream_Recv(LPCXSTR lpszClientAddr, XCHAR * *pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType)
{
	return m_PushStream.ModuleSession_PushStream_Recv(lpszClientAddr, pptszMsgBuffer, pInt_MsgLen, pInt_AVType, pInt_FrameType);
}
extern "C" bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr)
{
	return m_PushStream.ModuleSession_PushStream_ClientInsert(lpszClientAddr, lpszPullAddr);
}
extern "C" bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr)
{
	return m_PushStream.ModuleSession_PushStream_ClientDelete(lpszClientAddr, lpszPullAddr);
}
extern "C" bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<xstring>*pStl_ListClient)
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