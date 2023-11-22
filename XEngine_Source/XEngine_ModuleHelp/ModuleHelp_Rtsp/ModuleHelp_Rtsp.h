#pragma once
/********************************************************************
//    Created:     2023/11/22  16:32:33
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp\ModuleHelp_Rtsp.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Rtsp
//    File Base:   ModuleHelp_Rtsp
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTSP协议处理帮助函数
//    History:
*********************************************************************/

class CModuleHelp_Rtsp
{
public:
	CModuleHelp_Rtsp();
	~CModuleHelp_Rtsp();
public:
	bool ModuleHelp_Rtsp_GetSMSAddr(LPCXSTR lpszURLStr, XCHAR* ptszSMSAddr);
protected:
private:
};