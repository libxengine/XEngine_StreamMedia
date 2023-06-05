#include "pch.h"
#include "ModuleSession_Server/ModuleSession_Server.h"
#include "ModuleSession_Client/ModuleSession_Client.h"
#include "ModuleSession_Forward/ModuleSession_Forward.h"
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
CModuleSession_Client m_Client;
CModuleSession_Server m_Server;
CModuleSession_Forward m_Forward;
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
*                          导出会话模块                                          *
*********************************************************************************/
extern "C" bool ModuleSession_Client_Create(XNETHANDLE xhClient)
{
	return m_Client.ModuleSession_Client_Create(xhClient);
}
extern "C" bool ModuleSession_Client_Get(XNETHANDLE * pxhClient)
{
	return m_Client.ModuleSession_Client_Get(pxhClient);
}
extern "C" bool ModuleSession_Client_Exist(XNETHANDLE * pxhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Client.ModuleSession_Client_Exist(pxhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" bool ModuleSession_Client_Insert(XNETHANDLE xhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Client.ModuleSession_Client_Insert(xhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" bool ModuleSession_Client_DeleteAddr(LPCXSTR lpszDeviceAddr, XNETHANDLE * pxhClient, XCHAR * ptszDeviceNumber, int* pInt_Channel, bool * pbLive)
{
	return m_Client.ModuleSession_Client_DeleteAddr(lpszDeviceAddr, pxhClient, ptszDeviceNumber, pInt_Channel, pbLive);
}
extern "C" bool ModuleSession_Client_DeleteNumber(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Client.ModuleSession_Client_DeleteNumber(lpszDeviceNumber, nChannel, bLive);
}
extern "C" bool ModuleSession_Client_Destory()
{
	return m_Client.ModuleSession_Client_Destory();
}
extern "C" bool ModuleSession_Server_Create(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Server.ModuleSession_Server_Create(lpszDeviceNumber, nChannel, bLive);
}
extern "C" bool ModuleSession_Server_Destroy(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Server.ModuleSession_Server_Destroy(lpszDeviceNumber, nChannel, bLive);
}
extern "C" bool ModuleSession_Server_SetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive, XHANDLE xhToken)
{
	return m_Server.ModuleSession_Server_SetPush(lpszDeviceNumber, nChannel, bLive, xhToken);
}
extern "C" XHANDLE ModuleSession_Server_GetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive)
{
	return m_Server.ModuleSession_Server_GetPush(lpszDeviceNumber, nChannel, bLive);
}
/*********************************************************************************
*                          流转发导出会话模块                                    *
*********************************************************************************/
extern "C" bool ModuleSession_Forward_Create(LPCXSTR lpszPlay, XHANDLE xhToken, LPCXSTR lpszSMSPlay)
{
	return m_Forward.ModuleSession_Forward_Create(lpszPlay, xhToken, lpszSMSPlay);
}
extern "C" XHANDLE ModuleSession_Forward_Get(LPCXSTR lpszPlay)
{
	return m_Forward.ModuleSession_Forward_Get(lpszPlay);
}
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszPlay)
{
	return m_Forward.ModuleSession_Forward_Delete(lpszPlay);
}
extern "C" bool ModuleSession_Forward_List(MODULESESSION_FORWARDINFO * **pppSt_Forward, int* pInt_ListCount)
{
	return m_Forward.ModuleSession_Forward_List(pppSt_Forward, pInt_ListCount);
}

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