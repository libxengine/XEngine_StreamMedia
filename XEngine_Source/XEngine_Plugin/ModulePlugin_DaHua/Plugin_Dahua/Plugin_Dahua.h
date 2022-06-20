#pragma once
/********************************************************************
//    Created:     2022/06/14  14:02:39
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\Plugin_Dahua\Plugin_Dahua.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\Plugin_Dahua
//    File Base:   Plugin_Dahua
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     大华SDK实现
//    History:
*********************************************************************/
typedef struct  
{
	LLONG hSDKModule;
	NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY st_DevLoginInfo;
	NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY st_DevOutInfo;
	list<LLONG> *pStl_ListChannel;
}PLUGIN_SDKDAHUA;

class CPlugin_Dahua
{
public:
	CPlugin_Dahua();
	~CPlugin_Dahua();
public:
	BOOL PluginCore_SetCall(CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam = NULL);
	BOOL PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass);
	BOOL PluginCore_UnInit(XNETHANDLE xhToken);
	BOOL PluginCore_Play(XNETHANDLE xhToken, int nChannel);
	BOOL PluginCore_Stop(XNETHANDLE xhToken, int nChannel);
protected:
	static void CALLBACK PluginCore_CB_Disconnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_AutoConnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_RealData(LLONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser);
private:
	LPVOID m_lParam;
	CALLBACK_STREAMMEDIA_PLUGIN_AVDATA lpCall_AVData;
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA> stl_MapManager;
};
