#include "pch.h"
#include "ModuleSession_Server/ModuleSession_Server.h"
#include "ModuleSession_Client/ModuleSession_Client.h"
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
CModuleSession_Client m_Client;
CModuleSession_Server m_Server;
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
*                          导出会话模块                                          *
*********************************************************************************/
extern "C" BOOL ModuleSession_Client_Create(XNETHANDLE xhClient)
{
	return m_Client.ModuleSession_Client_Create(xhClient);
}
extern "C" BOOL ModuleSession_Client_Get(XNETHANDLE * pxhClient)
{
	return m_Client.ModuleSession_Client_Get(pxhClient);
}
extern "C" BOOL ModuleSession_Client_Exist(XNETHANDLE * pxhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_Client.ModuleSession_Client_Exist(pxhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_Client_Insert(XNETHANDLE xhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_Client.ModuleSession_Client_Insert(xhClient, lpszDeviceAddr, lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_Client_DeleteAddr(LPCTSTR lpszDeviceAddr, XNETHANDLE * pxhClient, TCHAR * ptszDeviceNumber, int* pInt_Channel, BOOL * pbLive)
{
	return m_Client.ModuleSession_Client_DeleteAddr(lpszDeviceAddr, pxhClient, ptszDeviceNumber, pInt_Channel, pbLive);
}
extern "C" BOOL ModuleSession_Client_DeleteNumber(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_Client.ModuleSession_Client_DeleteNumber(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_Client_Destory()
{
	return m_Client.ModuleSession_Client_Destory();
}
extern "C" BOOL ModuleSession_Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_Server.ModuleSession_Server_Create(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	return m_Server.ModuleSession_Server_Destroy(lpszDeviceNumber, nChannel, bLive);
}
extern "C" BOOL ModuleSession_Server_Insert(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	return m_Server.ModuleSession_Server_Insert(lpszDeviceNumber, nChannel, bLive, lpszMsgBuffer, nMsgLen);
}
extern "C" BOOL ModuleSession_Server_Get(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, TCHAR * ptszMsgBuffer, int* pInt_MsgLen)
{
	return m_Server.ModuleSession_Server_Get(lpszDeviceNumber, nChannel, bLive, ptszMsgBuffer, pInt_MsgLen);
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