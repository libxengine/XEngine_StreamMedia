#pragma once
/********************************************************************
//    Created:     2022/07/04  14:14:05
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Server\ModuleSession_Server.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Server
//    File Base:   ModuleSession_Server
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     服务器会话
//    History:
*********************************************************************/
typedef struct
{
	XHANDLE xhToken;
}SESSION_RTPPACKET;

class CModuleSession_Server
{
public:
	CModuleSession_Server();
	~CModuleSession_Server();
public:
	BOOL ModuleSession_Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Server_SetPush(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, XHANDLE xhToken);
	XHANDLE ModuleSession_Server_GetPush(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>> stl_MapServer;
};
