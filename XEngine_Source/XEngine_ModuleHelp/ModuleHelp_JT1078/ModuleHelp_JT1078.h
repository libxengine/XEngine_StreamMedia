#pragma once
/********************************************************************
//    Created:     2022/04/24  16:20:59
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_JT1078\ModuleHelp_JT1078.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_JT1078
//    File Base:   ModuleHelp_JT1078
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078相关帮助代码
//    History:
*********************************************************************/


class CModuleHelp_JT1078
{
public:
	CModuleHelp_JT1078();
	~CModuleHelp_JT1078();
public:
	bool ModuleHelp_JT1078_BCDToString(XBYTE bySIMNumber[6], XCHAR* ptszMsgBuffer);
	bool ModuleHelp_JT1078_StringToBCD(LPCXSTR lpszMsgBuffer, XBYTE* pbySIMNumber);
protected:
	void ModuleHelp_JT1078_2BytesToBCD(LPCXSTR lpszSource, XBYTE& chBCD);
	void ModuleHelp_JT1078_BCDTo2Bytes(XBYTE chBCD, XCHAR* ptszDest);
	int ModuleHelp_JT1078_BCDToInt(XBYTE chBCD);
	XBYTE ModuleHelp_JT1078_IntToBCD(XBYTE uszInt);
private:
};