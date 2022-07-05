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
#define XENGINE_STREAMMEDIA_PLUGIN_DAHUA_PACKET_SIZE 1024


typedef struct
{
	LLONG xhPlay;
	int nChannel;
	int nIndex;
}PLUGIN_SDKINFO;

typedef struct  
{
	LLONG hSDKModule;
	NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY st_DevLoginInfo;
	NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY st_DevOutInfo;

	shared_ptr<std::mutex> st_Locker;
	list<PLUGIN_SDKINFO>* pStl_ListChannel;
}PLUGIN_SDKDAHUA;
typedef struct
{
	shared_ptr<std::shared_mutex> st_Locker;
	list<PLUGIN_MQDATA> stl_ListMQData;
}PLUGIN_SDKMQLSIT;

class CPlugin_Dahua
{
public:
	CPlugin_Dahua();
	~CPlugin_Dahua();
public:
	BOOL PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, int nMaxPool);
	BOOL PluginCore_UnInit(XNETHANDLE xhToken);
	BOOL PluginCore_Play(XNETHANDLE xhToken, int nChannel);
	BOOL PluginCore_Stop(XNETHANDLE xhToken, int nChannel);
	BOOL PluginCore_GetData(XNETHANDLE xhToken, int nIndex, PLUGIN_MQDATA* pSt_MQData);
protected:
	static void CALLBACK PluginCore_CB_Disconnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_AutoConnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser);
	static void CALLBACK PluginCore_CB_RealData(LLONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser);
private:
	shared_mutex st_LockerData;
	shared_mutex st_LockerManage;
private:
	unordered_map<XNETHANDLE, unordered_map<int, PLUGIN_SDKMQLSIT> > stl_MapSDKData;
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA> stl_MapManager;
};
