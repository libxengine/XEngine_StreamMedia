#include "pch.h"
#include "Plugin_Dahua/Plugin_Dahua.h"
/********************************************************************
//    Created:     2022/06/14  15:15:00
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
BOOL SDKPlugin_IsErrorOccur = FALSE;
DWORD SDKPlugin_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CPlugin_Dahua m_PluginDH;
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD PluginCore_GetLastError()
{
	return SDKPlugin_dwErrorCode;
}
/*********************************************************************************
*                                导出函数定义                                    *
*********************************************************************************/
extern "C" BOOL PluginCore_Init(XNETHANDLE * pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, BOOL bDebug)
{
	return m_PluginDH.PluginCore_Init(pxhToken, lpszAddr, nPort, lpszUser, lpszPass, bDebug);
}
extern "C" BOOL PluginCore_UnInit(XNETHANDLE xhToken)
{
	return m_PluginDH.PluginCore_UnInit(xhToken);
}
extern "C" BOOL PluginCore_Play(XNETHANDLE xhToken, int nChannel, BOOL bAudio)
{
	return m_PluginDH.PluginCore_Play(xhToken, nChannel, bAudio);
}
extern "C" BOOL PluginCore_Stop(XNETHANDLE xhToken, int nChannel)
{
	return m_PluginDH.PluginCore_Stop(xhToken, nChannel);
}