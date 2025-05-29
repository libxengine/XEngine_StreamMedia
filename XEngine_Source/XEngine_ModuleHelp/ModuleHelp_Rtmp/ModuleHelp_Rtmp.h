#pragma once
/********************************************************************
//    Created:     2023/12/14  14:26:40
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtmp\ModuleHelp_Rtmp.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtmp
//    File Base:   ModuleHelp_Rtmp
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTMP协议处理帮助函数
//    History:
*********************************************************************/

typedef struct  
{
	XCHAR tszLiveName[XPATH_MAX];
	XCHAR tszURLStr[XPATH_MAX];
}RTMPPROTOCOL_CLIENTINFO;

class CModuleHelp_Rtmp
{
public:
	CModuleHelp_Rtmp();
	~CModuleHelp_Rtmp();
public:
	bool ModuleHelp_Rtmp_CreateSession(LPCXSTR lpszURLAddr, LPCXSTR lpszURLStr);
	bool ModuleHelp_Rtmp_GetSession(LPCXSTR lpszURLAddr, XCHAR* ptszLiveName);
	bool ModuleHelp_Rtmp_DeleteSession(LPCXSTR lpszURLAddr);
protected:
	shared_mutex st_Locker;
private:
	unordered_map<string, RTMPPROTOCOL_CLIENTINFO> stl_MapRTMPClient;
};