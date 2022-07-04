#pragma once
/********************************************************************
//    Created:     2022/07/04  14:40:00
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice\ModuleSession_SDKDevice.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice
//    File Base:   ModuleSession_SDKDevice
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SDK客户端会话
//    History:
*********************************************************************/
typedef struct
{
	int nChannel;
	BOOL bLive;
}MODULESESSION_SDKCLIENT;
typedef struct
{
	XNETHANDLE xhClient;
	XNETHANDLE xhToken;
	shared_ptr<shared_mutex> st_Locker;
	list<MODULESESSION_SDKCLIENT>* pStl_ListClient;
}MODULESESSION_LISTCLIENT;

class CModuleSession_SDKDevice
{
public:
	CModuleSession_SDKDevice();
	~CModuleSession_SDKDevice();
public:
	BOOL ModuleSession_SDKDevice_Create(XNETHANDLE xhToken, XNETHANDLE xhClient);
	BOOL ModuleSession_SDKDevice_Insert(XNETHANDLE xhToken, int nChannel, BOOL bLive);
	BOOL ModuleSession_SDKDevice_Delete(XNETHANDLE xhToken, int nChannel, BOOL bLive);
	BOOL ModuleSession_SDKDevice_Get(XNETHANDLE xhToken, int nChannel, BOOL bLive, XNETHANDLE *pxhClient);
	BOOL ModuleSession_SDKDevice_Destory();
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT> stl_MapClient;
};