#pragma once
/********************************************************************
//    Created:     2022/10/17  13:55:34
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward\ModuleSession_Forward.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward
//    File Base:   ModuleSession_Forward
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     媒体转发服务
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszPlayUrl[MAX_PATH];
	XHANDLE xhToken;
}MODULESESSION_FORWARD;

class CModuleSession_Forward
{
public:
	CModuleSession_Forward();
	~CModuleSession_Forward();
public:
	bool ModuleSession_Forward_Create(LPCXSTR lpszPlay, XHANDLE xhToken, LPCXSTR lpszSMSPlay);
	XHANDLE ModuleSession_Forward_Get(LPCXSTR lpszPlay);
	bool ModuleSession_Forward_Delete(LPCXSTR lpszPlay);
	bool ModuleSession_Forward_List(MODULESESSION_FORWARDINFO*** pppSt_Forward, int* pInt_ListCount);
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, MODULESESSION_FORWARD> stl_MapClient;
};