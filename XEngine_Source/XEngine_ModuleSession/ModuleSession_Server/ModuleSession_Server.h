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
	TCHAR* ptszMsgBuffer;
	int nMsgLen;
}SESSION_MSGBUFFER;
typedef struct
{
	XNETHANDLE xhToken;
	shared_mutex st_Locker;
	list<SESSION_MSGBUFFER>* pStl_ListBuffer;
}SESSION_RTPPACKET;

class CModuleSession_Server
{
public:
	CModuleSession_Server();
	~CModuleSession_Server();
public:
	BOOL ModuleSession_Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_Server_Insert(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, LPCTSTR lpszMsgBuffer, int nMsgLen);
	BOOL ModuleSession_Server_Get(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	BOOL ModuleSession_Server_SetPush(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, XNETHANDLE xhToken);
	BOOL ModuleSession_Server_GetPush(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, XNETHANDLE* pxhToken);
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>> stl_MapServer;
};
