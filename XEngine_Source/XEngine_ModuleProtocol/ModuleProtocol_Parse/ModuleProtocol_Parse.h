#pragma once
/********************************************************************
//    Created:     2022/09/28  15:31:39
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse\ModuleProtocol_Parse.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse
//    File Base:   ModuleProtocol_Parse
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议解析工具
//    History:
*********************************************************************/

class CModuleProtocol_Parse
{
public:
	CModuleProtocol_Parse();
	~CModuleProtocol_Parse();
public:
	bool ModuleProtocol_Parse_HTTPForward(LPCXSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszAVUrl);
private:
};