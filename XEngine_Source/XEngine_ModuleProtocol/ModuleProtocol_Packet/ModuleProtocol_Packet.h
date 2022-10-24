#pragma once
/********************************************************************
//    Created:     2022/09/28  15:29:48
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet\ModuleProtocol_Packet.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet
//    File Base:   ModuleProtocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议打包工具
//    History:
*********************************************************************/

class CModuleProtocol_Packet
{
public:
	CModuleProtocol_Packet();
	~CModuleProtocol_Packet();
public:
	BOOL ModuleProtocol_Packet_Comm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL, LPCTSTR lpszToken = NULL);
	BOOL ModuleProtocol_Packet_Create(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice);
	BOOL ModuleProtocol_Packet_Push(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, int nMsgLen, int nMsgType);
	BOOL ModuleProtocol_Packet_Destroy(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDev);
	BOOL ModuleProtocol_Packet_ForwardList(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, TCHAR*** ppptszForward, int nListCount);
private:
};