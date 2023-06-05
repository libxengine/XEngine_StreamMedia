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
extern "C" bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr)
{
	return m_PullStream.ModuleSession_PullStream_Insert(lpszClientAddr, lpszSMSAddr);
}
extern "C" bool ModuleSession_PullStream_Get(LPCXSTR lpszClientAddr, XCHAR * ptszSMSAddr)
{
	return m_PullStream.ModuleSession_PullStream_Get(lpszClientAddr, ptszSMSAddr);
}
extern "C" bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr)
{
	return m_PullStream.ModuleSession_PullStream_Delete(lpszClientAddr);
}
/*********************************************************************************
*                          推流导出会话模块                                      *
*********************************************************************************/
extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszSMSAddr, XNETHANDLE xhFLVStream)
{
	return m_PushStream.ModuleSession_PushStream_Create(lpszSMSAddr, xhFLVStream);
}
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszSMSAddr)
{
	return m_PushStream.ModuleSession_PushStream_Destroy(lpszSMSAddr);
}
extern "C" bool ModuleSession_PushStream_GetStreamForAddr(LPCXSTR lpszSMSAddr, XNETHANDLE * pxhFLVStream)
{
	return m_PushStream.ModuleSession_PushStream_GetStreamForAddr(lpszSMSAddr, pxhFLVStream);
}
extern "C" bool ModuleSession_PushStream_Send(LPCXSTR lpszSMSAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType)
{
	return m_PushStream.ModuleSession_PushStream_Send(lpszSMSAddr, lpszMsgBuffer, nMsgLen, nAVType, nFrameType);
}
extern "C" bool ModuleSession_PushStream_Recv(LPCXSTR lpszSMSAddr, XCHAR * *pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType)
{
	return m_PushStream.ModuleSession_PushStream_Recv(lpszSMSAddr, pptszMsgBuffer, pInt_MsgLen, pInt_AVType, pInt_FrameType);
}
extern "C" bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszSMSAddr, LPCXSTR lpszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_ClientInsert(lpszSMSAddr, lpszClientAddr);
}
extern "C" bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszSMSAddr, LPCXSTR lpszClientAddr)
{
	return m_PushStream.ModuleSession_PushStream_ClientDelete(lpszSMSAddr, lpszClientAddr);
}
extern "C" bool ModuleSession_PushStream_ClientList(LPCXSTR lpszSMSAddr, list<xstring>*pStl_ListClient)
{
	return m_PushStream.ModuleSession_PushStream_ClientList(lpszSMSAddr, pStl_ListClient);
}