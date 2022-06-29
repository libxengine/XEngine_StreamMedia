﻿#include "pch.h"
#include "ModuleSession_JT1078/ModuleSession_JT1078Client.h"
#include "ModuleSession_JT1078/ModuleSession_JT1078Server.h"
#include "ModuleSession_SDKDevice/ModuleSession_SDKDevice.h"
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
BOOL Session_IsErrorOccur = FALSE;
DWORD Session_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleSession_JT1078Client m_JTClient;
CModuleSession_JT1078Server m_JTServer;
CModuleSession_SDKDevice m_SDKDevice;
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleSession_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Session_dwErrorCode;
}
/*********************************************************************************
*                          JT1078导出会话模块                                    *
*********************************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Create(XNETHANDLE xhClient)
{
	return m_JTClient.ModuleSession_JT1078Client_Create(xhClient);
}
extern "C" BOOL ModuleSession_JT1078Client_Get(XNETHANDLE * pxhClient)
{
	return m_JTClient.ModuleSession_JT1078Client_Get(pxhClient);
}
extern "C" BOOL ModuleSession_JT1078Client_Exist(XNETHANDLE * pxhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_JTClient.ModuleSession_JT1078Client_Exist(pxhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_JT1078Client_Insert(XNETHANDLE xhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_JTClient.ModuleSession_JT1078Client_Insert(xhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_JT1078Client_DeleteAddr(LPCTSTR lpszDeviceAddr, XNETHANDLE * pxhClient, TCHAR * ptszDeviceNumber, int* pInt_Channel, BOOL * pbLive)
{
	return m_JTClient.ModuleSession_JT1078Client_DeleteAddr(lpszDeviceAddr, pxhClient, ptszDeviceNumber, pInt_Channel, pbLive);
}
extern "C" BOOL ModuleSession_JT1078Client_DeleteNumber(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_JTClient.ModuleSession_JT1078Client_DeleteNumber(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_JT1078Client_Destory()
{
	return m_JTClient.ModuleSession_JT1078Client_Destory();
}
extern "C" BOOL ModuleSession_JT1078Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_JTServer.ModuleSession_JT1078Server_Create(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_JT1078Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_JTServer.ModuleSession_JT1078Server_Destroy(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_JT1078Server_Insert(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_JTServer.ModuleSession_JT1078Server_Insert(lpszDeviceNumber, nChannel, bLive, lpszMsgBuffer, nMsgLen);
}
extern "C" BOOL ModuleSession_JT1078Server_Get(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_JTServer.ModuleSession_JT1078Server_Get(lpszDeviceNumber, nChannel, bLive, ptszMsgBuffer, pInt_MsgLen);
}
/*********************************************************************************
*                          设备SDK导出会话模块                                   *
*********************************************************************************/
extern "C" BOOL ModuleSession_SDKDevice_Create(XNETHANDLE xhToken, XNETHANDLE xhClient)
{
	return m_SDKDevice.ModuleSession_SDKDevice_Create(xhToken, xhClient);
}
extern "C" BOOL ModuleSession_SDKDevice_Insert(XNETHANDLE xhToken, int nChannel, BOOL bLive)
{
	return m_SDKDevice.ModuleSession_SDKDevice_Insert(xhToken, nChannel, bLive);
}
extern "C" BOOL ModuleSession_SDKDevice_Delete(XNETHANDLE xhToken, int nChannel, BOOL bLive)
{
	return m_SDKDevice.ModuleSession_SDKDevice_Delete(xhToken, nChannel, bLive);
}
extern "C" BOOL ModuleSession_SDKDevice_Get(XNETHANDLE xhToken, int nChannel, BOOL bLive, XNETHANDLE * pxhClient)
{
	return m_SDKDevice.ModuleSession_SDKDevice_Get(xhToken, nChannel, bLive, pxhClient);
}
extern "C" BOOL ModuleSession_SDKDevice_Destory()
{
	return m_SDKDevice.ModuleSession_SDKDevice_Destory();
}