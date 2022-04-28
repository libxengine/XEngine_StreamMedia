#pragma once
/********************************************************************
//    Created:     2022/04/28  14:19:06
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078\ModuleSession_JT1078Server.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078
//    File Base:   ModuleSession_JT1078Server
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078服务器会话
//    History:
*********************************************************************/
typedef struct
{
	XENGINE_RTPPACKETHDR2016 st_RTPHdr;
	XENGINE_RTPPACKETTAIL st_RTPTail;
	TCHAR* ptszMsgBuffer;
	int nMsgLen;
}SESSION_MSGBUFFER;
typedef struct
{
	shared_mutex st_Locker;
	list<SESSION_MSGBUFFER>* pStl_ListBuffer;
}SESSION_RTPPACKET;

class CModuleSession_JT1078Server
{
public:
	CModuleSession_JT1078Server();
	~CModuleSession_JT1078Server();
public:
	BOOL ModuleSession_JT1078Server_Create(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_JT1078Server_Destroy(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
	BOOL ModuleSession_JT1078Server_Insert(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail, LPCTSTR lpszMsgBuffer, int nMsgLen);
	BOOL ModuleSession_JT1078Server_Get(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive, TCHAR* ptszMsgBuffer, int* pInt_MsgLen);
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, unordered_map<int, unordered_map<BOOL, SESSION_RTPPACKET*>>> stl_MapServer;
};
