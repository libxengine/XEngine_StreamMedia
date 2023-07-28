#pragma once
/********************************************************************
//    Created:     2023/07/28  13:40:26
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Srt\ModuleHelp_SrtCore.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Srt
//    File Base:   ModuleHelp_SrtCore
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     ModuleHelp_SrtCore
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszClientAddr[128];
	SRTSOCKET hSocket;
}SRTCORE_CLIENTINFO;

class CModuleHelp_SrtCore
{
public:
	CModuleHelp_SrtCore();
	~CModuleHelp_SrtCore();
public:
	bool ModuleHelp_SrtCore_Start(int nPort);
	bool ModuleHelp_SrtCore_SetCallback(CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN fpCallePoll_Login, CALLBACK_NETCORE_SOCKET_NETEVENT_RECV fpCallePoll_Recv, CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE fpCallePoll_Leave, XPVOID lPLogin = NULL, XPVOID lPRecv = NULL, XPVOID lPLeave = NULL);
	bool ModuleHelp_SrtCore_Destory();
protected:
	bool ModuleHelp_SrtCore_Accept(SRTSOCKET hSRTSocket);
	bool ModuleHelp_SrtCore_Recv(SRTSOCKET hSocket);
protected:
	static XHTHREAD CALLBACK ModuleHelp_SrtCore_Thread(XPVOID lParam);
private:
	bool bRun = false;
	int hSRTEPoll = 0;
	SRTSOCKET hSRTSocket;
private:
	XPVOID m_lLogin;
	XPVOID m_lRecv;
	XPVOID m_lLeave;
	CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN lpCall_Login;
	CALLBACK_NETCORE_SOCKET_NETEVENT_RECV lpCall_Recv;
	CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE lpCall_Leave;
private:
	shared_mutex st_Locker;
	unique_ptr<thread> pSDTThread;
private:
	unordered_map<SRTSOCKET, SRTCORE_CLIENTINFO> stl_MapClients;
};