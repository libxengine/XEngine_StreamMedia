#pragma once
/********************************************************************
//    Created:     2022/07/04  14:05:56
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Stream\ModuleProtocol_Stream.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Stream
//    File Base:   ModuleProtocol_Stream
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     流媒体协议
//    History:
*********************************************************************/

class CModuleProtocol_Stream
{
public:
	CModuleProtocol_Stream();
	~CModuleProtocol_Stream();
public:
	BOOL ModuleProtocol_Stream_Create(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice);
	BOOL ModuleProtocol_Stream_Push(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, LPCTSTR lpszMsgBuffer, int nMsgLen, int nMsgType);
	BOOL ModuleProtocol_Stream_Destroy(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDev);
private:
};