#pragma once
/********************************************************************
//    Created:     2022/07/04  14:37:08
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Client\ModuleSession_Client.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Client
//    File Base:   ModuleSession_Client
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端会话管理
//    History:
*********************************************************************/
typedef struct
{
	TCHAR tszDeviceAddr[128];
	TCHAR tszDeviceNumber[128];
	int nChannel;
	BOOL bLive;
}MODULESESSION_CLIENT;
typedef struct
{
	shared_mutex st_Locker;
	list<MODULESESSION_CLIENT> stl_ListClient;
}MODULESESSION_LIST;

class CModuleSession_Client
{
public:
	CModuleSession_Client();
	~CModuleSession_Client();
public:
	BOOL ModuleSession_Client_Create(XHANDLE xhClient);
	BOOL ModuleSession_Client_Get(XHANDLE* pxhClient);
	BOOL ModuleSession_Client_Exist(XHANDLE* pxhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Client_Insert(XHANDLE xhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Client_DeleteAddr(LPCTSTR lpszDeviceAddr, XHANDLE* pxhClient = NULL, TCHAR* ptszDeviceNumber = NULL, int* pInt_Channel = NULL, BOOL* pbLive = NULL);
	BOOL ModuleSession_Client_DeleteNumber(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Client_Destory();
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<XHANDLE, MODULESESSION_LIST*> stl_MapClient;
};