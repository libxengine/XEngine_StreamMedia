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
	XHANDLE xhToken;
}MODULESESSION_FORWARD;

class CModuleSession_Forward
{
public:
	CModuleSession_Forward();
	~CModuleSession_Forward();
public:
	BOOL ModuleSession_Forward_Create(LPCTSTR lpszPlay, XHANDLE xhToken);
	XHANDLE ModuleSession_Forward_Get(LPCTSTR lpszPlay);
	BOOL ModuleSession_Forward_Delete(LPCTSTR lpszPlay);
protected:
private:
	shared_mutex st_Locker;
private:
	unordered_map<string, MODULESESSION_FORWARD> stl_MapClient;
};