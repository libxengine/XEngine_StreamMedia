﻿#include "pch.h"
#include "ModulePlugin_Core/ModulePlugin_Core.h"
/********************************************************************
//    Created:     2022/06/15  13:49:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件核心导出函数实现
//    History:
*********************************************************************/
BOOL ModulePlugin_IsErrorOccur = FALSE;
DWORD ModulePlugin_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModulePlugin_Core m_PluginCore;
//////////////////////////////////////////////////////////////////////////
//                       导出函数实现
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModulePlugin_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return ModulePlugin_dwErrorCode;
}
/*********************************************************************************
*                        插件框架导出函数定义                                       *
*********************************************************************************/
extern "C" BOOL ModulePlugin_Core_Insert(XNETHANDLE * pxhToken, LPCTSTR lpszPluginFile)
{
	return m_PluginCore.ModulePlugin_Core_Insert(pxhToken, lpszPluginFile);
}
extern "C" BOOL ModulePlugin_Core_Delete(XNETHANDLE xhToken)
{
	return m_PluginCore.ModulePlugin_Core_Delete(xhToken);
}
extern "C" BOOL ModulePlugin_Core_SetCall(XNETHANDLE xhToken, CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam)
{
	return m_PluginCore.ModulePlugin_Core_SetCall(xhToken, fpCall_AVData, lParam);
}
extern "C" BOOL ModulePlugin_Core_Init(XNETHANDLE xhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	return m_PluginCore.ModulePlugin_Core_Init(xhToken, lpszAddr, nPort, lpszUser, lpszPass);
}
extern "C" BOOL ModulePlugin_Core_UnInit(XNETHANDLE xhToken)
{
	return m_PluginCore.ModulePlugin_Core_UnInit(xhToken);
}
extern "C" BOOL ModulePlugin_Core_Play(XNETHANDLE xhToken, int nChannel)
{
	return m_PluginCore.ModulePlugin_Core_Play(xhToken, nChannel);
}
extern "C" BOOL ModulePlugin_Core_Stop(XNETHANDLE xhToken, int nChannel)
{
	return m_PluginCore.ModulePlugin_Core_Stop(xhToken, nChannel);
}