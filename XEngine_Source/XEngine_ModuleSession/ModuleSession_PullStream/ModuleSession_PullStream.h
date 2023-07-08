#pragma once
/********************************************************************
//    Created:     2023/06/05  16:10:01
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream\ModuleSession_PullStream.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream
//    File Base:   ModuleSession_PullStream
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     拉流端会话管理器
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszPushAddr[MAX_PATH];
}PULLSTREAM_CLIENTINFO;

class CModuleSession_PullStream
{
public:
	CModuleSession_PullStream();
	~CModuleSession_PullStream();
public:
	bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, LPCXSTR lpszPushAddr);
	bool ModuleSession_PullStream_GetSMSAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
	bool ModuleSession_PullStream_GetPushAddr(LPCXSTR lpszClientAddr, XCHAR* ptszPushAddr);
	bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, PULLSTREAM_CLIENTINFO*> stl_MapClient;
};