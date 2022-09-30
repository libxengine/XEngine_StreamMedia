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
#define XENGINE_STREAMMEDIA_PLUGIN_DAHUA_PACKET_SIZE 2048

typedef struct
{
	XNETHANDLE xhToken;
	shared_ptr<thread> pSTDThread;
	BOOL bAudio;
	LLONG xhPlay;
	int nChannel;
	LPVOID lClass;
	LPVOID lParam;
	CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER lpCall_SDKBuffer;
}PLUGIN_SDKINFO;

typedef struct  
{
	LLONG hSDKModule;
	NET_DEVICEINFO_Ex st_DevInfo;

	shared_ptr<std::shared_mutex> st_Locker;
	list<PLUGIN_SDKINFO*> stl_ListChannel;
}PLUGIN_SDKDAHUA;

class CPlugin_Dahua
{
public:
	CPlugin_Dahua();
	~CPlugin_Dahua();
public:
	BOOL PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, BOOL bPacket = TRUE, BOOL bDebug = FALSE);
	BOOL PluginCore_CBSet(XNETHANDLE xhToken, CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER fpCall_SDKBuffer, LPVOID lParam = NULL);
	BOOL PluginCore_UnInit(XNETHANDLE xhToken);
	BOOL PluginCore_Play(XNETHANDLE xhToken, int nChannel, BOOL bAudio = FALSE);
	BOOL PluginCore_Stop(XNETHANDLE xhToken, int nChannel);
protected:
	static void CALLBACK PluginCore_CB_Disconnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_AutoConnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_RealData(LLONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser);
protected:
	static DWORD CALLBACK PluginCore_Thread(LPVOID lParam);
private:
	BOOL m_bDebug;
	BOOL m_bPacket;
	FILE* pSt_VFile;
	FILE* pSt_AFile;
	shared_mutex st_LockerManage;
private:
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA> stl_MapManager;
};
