#pragma once
/********************************************************************
//    Created:     2022/07/05  10:07:07
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice\ModuleSession_SDKDevice.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice
//    File Base:   ModuleSession_SDKDevice
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SDK设备管理
//    History:
*********************************************************************/
typedef struct
{
	int nChannel;
	BOOL bLive;
}MODULESESSION_SDKCLIENT;
typedef struct
{
	shared_mutex st_Locker;
	unordered_map<XHANDLE, list<MODULESESSION_SDKCLIENT> > stl_MapClient;
}MODULESESSION_SDKLIST;

class CModuleSession_SDKDevice
{
public:
	CModuleSession_SDKDevice();
	~CModuleSession_SDKDevice();
public:
	BOOL ModuleSession_SDKDevice_Create(XNETHANDLE xhDevice);
	BOOL ModuleSession_SDKDevice_InsertClient(XNETHANDLE xhDevice, XHANDLE xhClient);
	BOOL ModuleSession_SDKDevice_InsertDevice(XNETHANDLE xhDevice, XHANDLE xhClient, int nChannel, BOOL bLive);
	XHANDLE ModuleSession_SDKDevice_GetIdleClient(XNETHANDLE xhDevice);
	XHANDLE ModuleSession_SDKDevice_GetClient(XNETHANDLE xhDevice, int nChannel, BOOL bLive);
	XHANDLE ModuleSession_SDKDevice_Delete(XNETHANDLE xhDevice, int nChannel, BOOL bLive);
	BOOL ModuleSession_SDKDevice_Destory();
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*> stl_MapClient;
};