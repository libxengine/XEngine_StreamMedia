﻿#pragma once
/********************************************************************
//    Created:     2022/06/14  13:39:03
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Core\ModulePlugin_Core.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Core
//    File Base:   ModulePlugin_Core
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件核心架构定义
//    History:
*********************************************************************/
typedef BOOL(*FPCall_PluginCore_SetCall)(CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam);
typedef BOOL(*FPCall_PluginCore_Init)(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
typedef BOOL(*FPCall_PluginCore_UnInit)(XNETHANDLE xhToken);
typedef BOOL(*FPCall_PluginCore_GetInfo)(XNETHANDLE xhToken, TCHAR* ptszModuleName, TCHAR* ptszModuleMode);
typedef BOOL(*FPCall_PluginCore_Play)(XNETHANDLE xhToken, int nChannel);
typedef BOOL(*FPCall_PluginCore_Stop)(XNETHANDLE xhToken, int nChannel);
typedef DWORD(*FPCall_PluginCore_GetLastError)();

typedef struct 
{
	HMODULE mhFile;
	XNETHANDLE xhModule;
	TCHAR tszModuleFile[MAX_PATH];
	TCHAR tszModuleName[MAX_PATH];
	TCHAR tszModuleMode[MAX_PATH];

	BOOL(*fpCall_PluginCore_SetCall)(CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam);
	BOOL(*fpCall_PluginCore_Init)(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
	BOOL(*fpCall_PluginCore_UnInit)(XNETHANDLE xhToken);
	BOOL(*fpCall_PluginCore_GetInfo)(XNETHANDLE xhToken, TCHAR* ptszModuleName, TCHAR* ptszModuleMode);
	BOOL(*fpCall_PluginCore_Play)(XNETHANDLE xhToken, int nChannel);
	BOOL(*fpCall_PluginCore_Stop)(XNETHANDLE xhToken, int nChannel);
	DWORD(*fpCall_PluginCore_GetLastError)();
}PLUGINCORE_FRAMEWORK, * LPPLUGINCORE_FRAMEWORK;

class CModulePlugin_Core
{
public:
	CModulePlugin_Core();
	~CModulePlugin_Core();
public:
	BOOL ModulePlugin_Core_Insert(XNETHANDLE* pxhToken, LPCTSTR lpszPluginFile);
	BOOL ModulePlugin_Core_Delete(XNETHANDLE xhToken);

	BOOL ModulePlugin_Core_SetCall(XNETHANDLE xhToken, CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam = NULL);
	BOOL ModulePlugin_Core_Init(XNETHANDLE xhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
	BOOL ModulePlugin_Core_UnInit(XNETHANDLE xhToken);
	BOOL ModulePlugin_Core_Play(XNETHANDLE xhToken, int nChannel);
	BOOL ModulePlugin_Core_Stop(XNETHANDLE xhToken, int nChannel);
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK> stl_MapFrameWork;
};