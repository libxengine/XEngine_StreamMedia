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
	XCHAR tszDeviceAddr[128];
	XCHAR tszDeviceNumber[128];
	int nChannel;
	bool bLive;
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
	bool ModuleSession_Client_Create(XNETHANDLE xhClient);
	bool ModuleSession_Client_Get(XNETHANDLE* pxhClient);
	bool ModuleSession_Client_Exist(XNETHANDLE* pxhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
	bool ModuleSession_Client_Insert(XNETHANDLE xhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
	bool ModuleSession_Client_DeleteAddr(LPCXSTR lpszDeviceAddr, XNETHANDLE* pxhClient = NULL, XCHAR* ptszDeviceNumber = NULL, int* pInt_Channel = NULL, bool* pbLive = NULL);
	bool ModuleSession_Client_DeleteNumber(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
	bool ModuleSession_Client_Destory();
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<XNETHANDLE, MODULESESSION_LIST*> stl_MapClient;
};