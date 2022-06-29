#pragma once
/********************************************************************
//    Created:     2022/04/25  14:12:43
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_JT1078\ModuleProtocol_JT1078.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_JT1078
//    File Base:   ModuleProtocol_JT1078
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078协议
//    History:
*********************************************************************/


class CModuleProtocol_JT1078
{
public:
	CModuleProtocol_JT1078();
	~CModuleProtocol_JT1078();
public:
	BOOL ModuleProtocol_JT1078_StreamCreate(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice);
	BOOL ModuleProtocol_JT1078_StreamPush(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, LPCTSTR lpszMsgBuffer, int nMsgLen, int nMsgType);
	BOOL ModuleProtocol_JT1078_StreamDestroy(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDev);
private:
};